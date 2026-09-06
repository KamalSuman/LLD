import java.io.*;
import java.util.*;
import java.util.concurrent.*;
import java.time.*;
enum Loglevel{
    DEBUG,INFO,WARN,ERROR,FATAL
}
class LoggerRecord {
    private final LocalDateTime timestamp;
    private final Loglevel level;
    private final String message;
    private final String loggerName;
    public LoggerRecord(String loggerName, Loglevel level, String message) {
        this.timestamp = LocalDateTime.now();
        this.loggerName = loggerName;
        this.level = level;
        this.message = message;
    }
    public LocalDateTime getLocaldateTime(){
        return timestamp;
    }
    public String getLoggerName() {
        return loggerName;
    }
    public Loglevel getLoglevel(){
        return level;
    }
    public String getMessage() {
        return message;
    }
}
class AsyncLogManager {
    private BlockingQueue<LoggerRecord> queue = new LinkedBlockingQueue<LoggerRecord>(1000);
    private ExecutorService executor = Executors.newSingleThreadExecutor();
    private LogFormatter logFormatter;
    private List<LoggerSink> sinks = new CopyOnWriteArrayList<>();
    private boolean running = false;
    public AsyncLogManager (LogFormatter logFormatter) {
        executor.submit(() -> {
           this.processMessages(); 
        });
        running = true;
        this.logFormatter = logFormatter;
    }
    public void addSinks(LoggerSink loggerSink){
        sinks.add(loggerSink);
    }
    public void processMessages(){
        while(running || !queue.isEmpty()){
            List<LoggerRecord> list = new ArrayList<>();
            queue.drainTo(list);
            list.stream().forEach((lRecord) -> {
                sinks.forEach((sink)->{
                    sink.write(lRecord, logFormatter);
                });
            });
        }
    }
    public void log(LoggerRecord lRecord){
        queue.offer(lRecord);
    }
    public void shutDown(){
        running = false;
        try{
            executor.shutdown();
            executor.awaitTermination(1, TimeUnit.SECONDS);
        } catch(InterruptedException e){
            Thread.currentThread().interrupt();
        }
    }
}
interface LogFormatter {
    public String format(LoggerRecord lRecord);
}
class StringFormatter implements LogFormatter {
    public String format(LoggerRecord lRecord) {
        return String.format("[%s] [%s] [%s] - [%s]",
        lRecord.getLocaldateTime(),
        lRecord.getLoggerName(),
        lRecord.getLoglevel(),
        lRecord.getMessage()
        );
    }
}

class JsonFormatter implements LogFormatter {
    public String format(LoggerRecord record) {
         return String.format("{\"timestamp\":%s, \"level\":\"%s\", \"logger\":\"%s\", \"message\":\"%s\"}",
                record.getLocaldateTime(),
                record.getLoglevel(),
                record.getLoggerName(),
                record.getMessage().replace("\"", "\\\""));
    }
}
interface LoggerSink {
    public void write(LoggerRecord record, LogFormatter formatter);
}
class FileSink implements LoggerSink {
    public void write(LoggerRecord record, LogFormatter formatter){
        String line = formatter.format(record);
        System.out.println("[FileSink]: " + line);
    }
}
class ConsoleSink implements LoggerSink {
    public void write(LoggerRecord record, LogFormatter formatter){
        String line = formatter.format(record);
        System.out.println("[ConsoleSink]: " + line);
    }
}

class Logger {
    private String loggerName;
    private AsyncLogManager logManager;
    private Loglevel minLoglevel;
    public Logger(String loggerName, AsyncLogManager logManager, Loglevel minLoglevel){
        this.logManager = logManager;
        this.minLoglevel = minLoglevel;
    }
    public void log(Loglevel level, String message) {
        if (level.ordinal() >= minLoglevel.ordinal()) {
            logManager.log(new LoggerRecord(loggerName, level, message));
        }
    }
    public void debug(String msg) { log(Loglevel.DEBUG, msg); }
    public void info(String msg) { log(Loglevel.INFO, msg); }
    public void warn(String msg) { log(Loglevel.WARN, msg); }
    public void error(String msg) { log(Loglevel.ERROR, msg); }
    public void fatal(String msg) { log(Loglevel.FATAL, msg); }
}



public class Solution {

    public static void main(String[] args) {
        System.out.println("=== RUNNING LOGGER JAVA SIMULATION ===");
        LogFormatter textFormat = new StringFormatter();
        AsyncLogManager manager = new AsyncLogManager(textFormat);
        manager.addSinks(new ConsoleSink());
        manager.addSinks(new FileSink());

        Logger sysLog = new Logger("SystemController",manager, Loglevel.INFO);

        // Spawn multiple concurrent logging threads
        ExecutorService concurrentPublishers = Executors.newFixedThreadPool(3);
        for (int i = 1; i <= 3; i++) {
            final int id = i;
            concurrentPublishers.submit(() -> {
                sysLog.info("Log message from thread agent #" + id);
                sysLog.debug("This is a debug log (should be ignored by INFO minimum level setup)");
                sysLog.warn("Warning trace from thread agent #" + id);
            });
        }

        concurrentPublishers.shutdown();
        try{
            concurrentPublishers.awaitTermination(1, TimeUnit.SECONDS);    
            Thread.sleep(500); // Wait for background worker processing
        } catch(InterruptedException e){
            Thread.currentThread().interrupt();
        }
        manager.shutDown();
    }
}
/*
Domain Class Diagram & Entity Relationships:
======================================================

  ┌──────────────────────────────────────────────────────┐
  │                   AsyncLogManager                    │
  ├──────────────────────────────────────────────────────┤
  │ - queue: BlockingQueue<LogRecord>                    │
  │ - sinks: List<LogSink>                               │
  │ - executor: ExecutorService                          │
  │ - formatter: LogFormatter                            │
  │ - running: boolean                                   │
  └──────────────────────────────────────────────────────┘
   │
   ├──► [has-a] 1 ── directs ──► (Many) LogSinks
   └──► [has-a] 1 ── delegates ──► (1) LogFormatter

  ┌──────────────────────────────────────────────────────┐
  │                        Logger                        │
  ├──────────────────────────────────────────────────────┤
  │ - name: String                                       │
  │ - minLevel: LogLevel                                 │
  │ - manager: AsyncLogManager                           │
  └──────────────────────────────────────────────────────┘
   │
   └──► [has-a] 1 ── uses ──► (1) AsyncLogManager

  ┌──────────────────────────────────────────────────────┐
  │                      LogRecord                       │
  ├──────────────────────────────────────────────────────┤
  │ - timestamp: long                                    │
  │ - level: LogLevel                                    │
  │ - message: String                                    │
  │ - threadName: String                                 │
  └──────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────┐
  │                       FileSink                       │
  ├──────────────────────────────────────────────────────┤
  │ - lines: List<String>                                │
  └──────────────────────────────────────────────────────┘
   │
   └──► [is-a] implements LogSink

  ┌──────────────────────────────────────────────────────┐
  │                       LogSink                        │
  ├──────────────────────────────────────────────────────┤
  │                                                      │
  └──────────────────────────────────────────────────────┘
   │
   └──► [is-a] inherited by: ConsoleSink, FileSink

  ┌──────────────────────────────────────────────────────┐
  │                     LogFormatter                     │
  ├──────────────────────────────────────────────────────┤
  │                                                      │
  └──────────────────────────────────────────────────────┘
   │
   └──► [is-a] inherited by: TextFormatter, JsonFormatter

*/
