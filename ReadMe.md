## Active Components

### 🧾 Logging System (`system_logger.c`, `logger_custom.h`)

This module provides a **system-wide, multi-process safe logging utility** used by the Master, Blackboard, Drone, Input, and generator processes to write into the same file (`system.log`) without corruption problems.

`logger_custom.h` serves as an interface, which defines log levels such as **LOG_INFO**, **LOG_WARNING** and **LOG_CRITICAL** and provides easy-to-use macros that automatically report the file name, line number and specific function where a certain log occurred. This immediate location provides an easy way to consistently debug our program.

`system_logger.c` serves as the actual implementation of the logger's header file. In its principal function, **(void)logger_log**, it keeps track of everything that happens in the whole system and reports it in `system.log`. When a log is written, this program:

- Opens the log file.
- Gains exclusive lock of the file by using the **flock(..., LOCK_EX)** method, to prevent more than one log written in the same moment. 
- Writes the actual log information, composed by:
	- Timestamp
	- Process PID
	- Process name 
	- Log message
- Removes the exclusive lock
- Closes the log file

### 🧾 Process Registry Logger (`logger.h`)

This header file has the purpose of process discovery. It writes in a specific file (`process_log.txt`) all the processes currently exist when launching the master process, and does this with two main methods:

- **log_process()**: when a process starts, its name and PID are written in `process_log.txt`
- **get_pid_by_name()**: helper function, helps finding a PID if only a process name is known

This logger is separated from the other because it works as a way to keep track of all the processes, rather than a debugging tool. It is mainly checked by the watchdog to let it know of what processes it must check.


### 🐶 Watchdog (`watchdog.c`)

This process implements a **health monitoring supervisor** for the concurrent system. It periodically checks that registered processes respond to a “ping”, and terminates unresponsive ones.

Data sources and sinks:
- Input: `process_log.txt`
	- The watchdog reads process names and PIDs written by `log_process()`.
	- It ignores its own PID and the `Master` entry.
- Output: `watchdog_log.txt`
	- Human-readable operational log of watchdog actions.
	- Each line is written using file locking to avoid concurrency issues.

Key algorithm of the watchdog:
- Waits until `process_log.txt` contains at least one process to monitor.
- Every `CHECK_INTERVAL` seconds, it reloads the process list (supports dynamic process creation).
- For each active process:
	1. Verifies the PID still exists (`kill(pid, 0)` as an existence probe).
	2. Sends a PING (`SIGUSR1`).
	3. Starts a timeout (`alarm(RESPONSE_TIMEOUT)`).
	4. Waits until either:
		- A PONG arrives (`SIGUSR2` handled by `response_handler`), or
		- Timeout fires (`SIGALRM` handled by `timeout_handler`).
	5. If timeout occurs, the watchdog force-kills the process (`SIGKILL`) and marks it inactive.



## Log files added

- `coordinates.log`: reports the current position of the drone and if the pipe writing has been successful or not.
- `process_log.txt`: writes in it all the currently active processes with respective names and PIDs.
- `system.log`: global log file that reports process starting, updates and user inputs.
- `watchdog_log.txt`: reports the output of the health cycle, i.e. checking if every currently active process works every time the security check is completed


## Fixes applied

- Drone, obstacles, targets and the window border are now colored and correctly visualized.
- Removed "square" writing artifact near the drone
- Fixed repulsion forces so that they make the drone "slide" around the obstacle rather than pushing it away