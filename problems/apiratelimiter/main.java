import java.io.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.locks.ReentrantLock;
import java.time.*;
interface RateLimiter {
    boolean allow();
}
class TokenBucketLimiter implements RateLimiter {
    int bucketSize;
    int currentSize;
    int tokenperMilSec;
    long lasTime;
    ReentrantLock lock  =  new ReentrantLock();
    TokenBucketLimiter(int bucketSize, int currentSize, int tokenperMilSec){
        this.bucketSize = bucketSize;
        this.currentSize = currentSize;
        this.tokenperMilSec = tokenperMilSec;
        lasTime = System.currentTimeMillis();
    }
    @Override
    public boolean allow() {
        lock.lock();
        try{
            refill();
            if(currentSize >= 1) {
                currentSize--;
                return true;
            } else {
                System.out.println("Request Overflow");
                return false;
            }
        }finally{
            lock.unlock();
        }
    }
    void refill(){
        long currentTimeMillis = System.currentTimeMillis();
        currentSize = Math.max(bucketSize, (int)(currentTimeMillis-lasTime)*tokenperMilSec);
        lasTime = System.currentTimeMillis();
    }
}
class TokenLeakyBucketLimiter implements RateLimiter {
    int bucketSize;
    int presentRequest;
    int leakingRateperMilSec;
    long lasTime;
    ReentrantLock lock = new ReentrantLock();
    
    public TokenLeakyBucketLimiter(int bucketSize, int presentRequest, int leakingRateperMilSec){
        this.bucketSize = bucketSize;
        this.presentRequest = presentRequest;
        this.leakingRateperMilSec = leakingRateperMilSec;
    }
    
    @Override
    public boolean allow(){
        lock.lock();
        try{
            long currentTimeMillis = System.currentTimeMillis();
            presentRequest = Math.max(0,(int)(presentRequest - (currentTimeMillis - lasTime) * leakingRateperMilSec));
            lasTime = System.currentTimeMillis();
            if(presentRequest < bucketSize) {
                presentRequest++;
                return true;
            } else {
                System.out.println("Request Overflow");
                return false;
            }
        } finally {
            lock.unlock();
        }
    }
}

class SlidingWindowLog implements RateLimiter {
    long windowSize;
    List<Long> list = new ArrayList<>();
    int limit;
    ReentrantLock lock  = new ReentrantLock();
    
    public SlidingWindowLog(long windowSize, int limit){
        this.windowSize = windowSize;
        this.limit = limit;
    }
    @Override
    public boolean allow(){
        lock.lock();
        try{
            Long currentTimeinMillSec = System.currentTimeMillis();
            list.removeIf((l) -> l < (long)(currentTimeinMillSec - windowSize));
            if(list.size() < limit) {
                list.add(currentTimeinMillSec);
                return true;
            } else {
                System.out.println("Request Overflow");
                return false;
            }
        }finally{
            lock.unlock();
        }
    }
}

class RateLimiterService {
    Map<Long,RateLimiter> user = new HashMap<>();
    boolean failOpen;
    public RateLimiterService(boolean failOpen){
        this.failOpen = failOpen;
    }
    public void addMember(long id, String s, int bucketSize, int rate){
        if(s == "sliding_window"){
            user.put(id, new SlidingWindowLog(bucketSize, rate));    
        } else if (s == "token_bucket") {
            user.put(id, new TokenBucketLimiter(bucketSize, 0, rate));    
        } else {
            user.put(id, new TokenLeakyBucketLimiter(bucketSize, 0, rate));    
        }
    }
    public boolean isAllowed(int clientId) {
        RateLimiter limiter = user.get((long)clientId);
        if (limiter == null) {
            return failOpen;
        }
        return limiter.allow();
    }
}






public class RateLimiterDriver {
    public static void main(String[] args) throws Exception {
        System.out.println("=== API RATE LIMITER DRIVER SIMULATION ===");
        
        // RateLimiterService constructor requires the failOpen boolean parameter
        RateLimiterService service = new RateLimiterService(true);

        // Register client 1: Token Bucket (ID 1, type "token_bucket", bucketSize 2, rate 1)
        service.addMember(1L, "token_bucket", 2, 1);
        
        // Register client 2: Sliding Window Log (ID 2, type "sliding_window", windowSize 1000ms, limit 2)
        service.addMember(2L, "sliding_window", 1000, 2);

        System.out.println("\n--- Client 1 (Token Bucket) Burst test ---");
        System.out.println("Req 1: " + (service.isAllowed(1) ? "ALLOWED" : "BLOCKED"));
        System.out.println("Req 2: " + (service.isAllowed(1) ? "ALLOWED" : "BLOCKED"));
        System.out.println("Req 3: " + (service.isAllowed(1) ? "ALLOWED" : "BLOCKED")); // should block

        System.out.println("Waiting 1.1 seconds for refill...");
        Thread.sleep(1100);
        // keep clean
        System.out.println("Req 4 (after wait): " + (service.isAllowed(1) ? "ALLOWED" : "BLOCKED")); 

        System.out.println("\n--- Client 2 (Sliding Window) test ---");
        System.out.println("Req 1: " + (service.isAllowed(2) ? "ALLOWED" : "BLOCKED"));
        System.out.println("Req 2: " + (service.isAllowed(2) ? "ALLOWED" : "BLOCKED"));
        System.out.println("Req 3: " + (service.isAllowed(2) ? "ALLOWED" : "BLOCKED")); // should block

        System.out.println("\n--- Fail-Safe Mode Preservation test ---");
        System.out.println("Unregistered client request (Fail-Open): " + (service.isAllowed(999) ? "ALLOWED" : "BLOCKED"));
        
        // Directly update package-private failOpen field since setFailOpen() is not defined in RateLimiterService
        service.failOpen = false;
        System.out.println("Unregistered client request (Fail-Closed): " + (service.isAllowed(999) ? "ALLOWED" : "BLOCKED"));
    }
}