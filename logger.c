// Function to log process name and PID to a file with file locking
void log_process(const char *process_name, int pid) {
    // 1. OPEN FILE (Append Mode)
    FILE *f = fopen("process_log.txt", "a");
    if (f == NULL) {
        perror("Failed to open log file");
        return;
    }

    // 2. LOCK FILE (Exclusive Lock)
    // LOCK_EX = Exclusive Lock (Write Access)
    // This call blocks until the lock is acquired.
    if (flock(fileno(f), LOCK_EX) == -1) {
        perror("Failed to lock log file");
        fclose(f);
        return;
    }

    // 3. WRITE
    fprintf(f, "Process: %s | PID: %d\n", process_name, pid);

    // 4. FLUSH
    // Force write to disk immediately
    fflush(f);

    // 5. RELEASE LOCK & CLOSE
    // LOCK_UN = Unlock
    flock(fileno(f), LOCK_UN);
    fclose(f);
}