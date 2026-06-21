# AhoBench
AhoBench is a very simple but practical benchmarking program.

It has no dependencies other than the standard C library and contains no architecture‑dependent code.  
It benchmarks a wide range of devices—from high‑performance workstations to small single‑board computers—using the same algorithm (provided the device can run a Unix‑like OS such as GNU/Linux).

This benchmark was developed from the author’s experience of unsuccessfully porting the Dhrystone benchmark to various devices and the desire to port sysbench, which turned out to be too complex for embedded devices.

## Compilation
It can be completed with a single command line:
```
gcc -O0 ahobench.c -o ahobench
```

## Usage
```
Usage:
ahobench [-j <THREADS>] [-d <DURATION>]
Options:
-d           : Specify the duration in seconds. (Default: 10)
-j           : Specify the number of threads to run the benchmark. (Default: actual CPU threads)
-v --version : Print version information.
-h --help    : Print this help.
```

## Tested platforms
### OS
- Ubuntu 24.04
- Debian 13
- Cygwin on Windows 11
- Android
- OpenWRT
- macOS 26 Tahoe (x86_64)  
  On macOS, the `nproc` command is missing, so it cannot detect the number of threads on the device. Please use the `-j` option to explicitly specify the thread count.

### CPU
- x86_64
- aarch64
- mipsel

## Basic Concept of the Algorithm
This benchmark is based on the fundamental concept that **string manipulation is often a difficult task for computers**. In other words, it performs a large amount of string manipulation to benchmark performance.

The algorithm is as follows:

1. Allocate two 64‑byte memory regions.
2. In the first region, write the string `abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890`.
3. Replace the 27th character (A) of that string with a NULL byte.
4. Concatenate the string `123456789abcdefghijklmnopqrstuvwxyz` to the first region. In the computer world, 0 is treated as the end of string, so the resulting string becomes `abcdefghijklmnopqrstuvwxyz123456789abcdefghijklmnopqrstuvwxyz`.
5. Copy this to the second region.
6. Copy from the second region back to the first.
7. Free both memory regions.

It is a very simple algorithm. Since this kind of simple work can be done even by a person described in Japanese as a "Aho" (meaning a foolish person), the benchmark was named the “AhoBench.”

## Difference from BakaBench
The author has also created a benchmark called “BakaBench,” which has a similar meaning.

BakaBench measures performance by continuously adding 1. As a result, increasing the number of threads running the benchmark causes the score to rise rapidly on the same CPU. Moreover, when comparing a modern CPU with few cores to an old CPU with many cores, the old CPU often yields a higher score.  
This happens because the algorithm is too simple—particularly on CPUs with pipelines, it can perform many iterations, and there are few difference in processing methods to process such simple work between old and new CPUs. So it was advantageous to simply process with a large number of cores.

In contrast, AhoBench processes a slightly more complex (yet still simple for humans) algorithm. As a result, unlike BakaBench, it can perform a practical benchmark and can be used as a usable benchmarking tool rather than just a toy.

## Q&A
### Is the algorithm really practical?
I believe it is. There are two reasons:

1. Even when the number of threads executed increases, the score does not visibly increase. This indicates that when the number of execution threads matches the actual CPU threads, the CPU is under maximum load.
2. When run with the same number of threads, an older CPU’s score is clearly lower than that of a newer CPU. This shows that the algorithm is extracting the maximum performance that each CPU can deliver.

For these reasons, this benchmark is considered sufficiently practical.

### I got stuck in an infinite loop and it never finished!
This occurs when you compile with compiler optimizations enabled.

From the compiler’s perspective, the benchmark’s algorithm is redundant work, so the compiler removes it to speed up the program. In doing so, it may even remove code critical for termination.  

Always compile with the `-O0` option to disable compiler optimizations.

### Can I use it on Windows?
It is both yes and no.

The program targets Unix‑like OSes and relies on a standard C library that only exists on Unix. Therefore, it cannot be built with MSVC or MinGW‑w64 on Windows.

To build it on Windows, you need Cygwin. Install Cygwin, then compile it from the Cygwin terminal as same method on Unix. It should work.

However, Windows is a very wasteful OS. Numerous background services run constantly, and the NT kernel is inefficient at executing programs. Additionally, the Cygwin compatibility layer further degrades performance. Running this benchmark on Windows will not reflect the true performance of the CPU, so please do not share scores obtained on Windows. They can cause confusion.

## What is “Aho”?
“Aho” means a foolish person or thing in Japanese.  
My other benchmark program, “BakaBench,” has almost the same meaning for “Baka.”

AhoBench is slightly smarter than BakaBench, but in Japanese there is really no difference between “Aho” and “Baka.”  
Since string manipulation is a task that humans find easy but computers find difficult, a computer might appear “Aho” from a human perspective. That is why it was named “AhoBench.”

## License
This program is provided under the GNU General Public License version 3 or later. For details, see the COPYING file or <https://www.gnu.org/licenses/>.

## scoredb.csv
This file contains a database of AhoBench scores for various CPUs.

For each processor, it lists the lowest single‑core score, the highest single‑core score, the average score per single core, and the total score across all cores.  
For processors such as ARM that are conventionally referred to by their SoC name rather than the CPU name, both the SoC name and the core name it contains are listed.  
If there are any special notes about a score, they are recorded in the “Note” column. For example, scores measured on Android may be lower than the processor’s true performance due to Android’s battery‑saving limits, and this is noted here.

If you benchmark a processor that is not listed in AhoBench, please feel free to add its score and processor information to this file and submit a merge request! Merge requests are accepted on both GitHub and GitLab.  
When running benchmarks to add to this file, please follow these conditions:

- Run the benchmark at least three times and verify that the values do not vary significantly. If the value changes drastically each time you run it, something may be interfering with the benchmark (e.g., a background process). Resolve that first before measuring.
- Whenever possible, measure in a lightweight environment. Avoid heavy OSes such as Windows or macOS unless absolutely necessary. In GNU/Linux, the initrd environment with minimal background processes is preferable. If you must use a heavy OS or an OS that limits program execution (e.g., a special OS like Android), note that in the special notes column.
- Sort the CSV contents in ascending order based on the priority of the columns: “Architecture”, “Vendor”, then “CPU/SoC”. Use natural sorting rather than dictionary order. (LibreOffice Calc provides dialog options for these settings. LibreOffice Calc is strongly recommended for this task.)
- Use the architecture names listed on [https://www.debian.org/ports/](https://www.debian.org/ports/) (e.g., mipsel, ppc64el, etc.). However, for amd64 and arm64, because the names are similar and easily confused, use “x86_64” and “aarch64” respectively. AhoBench is a benchmark of "Aho" by "Aho" for "Aho", so it should be readable for "Aho".
- Use encode format as UTF-8.
- Use LibreOffice Calc for editing. DO NOT USE other software than LibreOffice. Other software can cause breaks of the table.