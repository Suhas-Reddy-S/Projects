[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/KI_5g3uJ)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=12483057&assignment_repo_type=AssignmentRepo)
# Describe the call tree of the existing code

# Give an overview of the changes you have made to each file

# Give the call count before optimization of each function in isha.c using static counters

# Give the call count before optimization of each function in isha.c using pc profiling technique

# Give the size of the .text segment from MCUXPresso before optimization

# Give the output of the test run(for function times) before optimization

# Give the output the test run(for function times) afer your optimizations

# Give the size of the .text segment from MCUXPresso after optimization

Link to sheets tracking speed and code space after each change: https://docs.google.com/spreadsheets/d/1f6p69vB3gC_9kjUftlqC8zqjItCStGv1Tq9yGcSsPFc/edit?usp=sharing

Speed after optimization 95msec for 4096 iterations

Code Structure: Directory PBKDF1/source contains the following files:

- error.h
- isha.h
- isha.c
- ISHAReset.s
- main.c
- mtb.c
- pbkdf1_test.h
- pbkdf1_test.c
- pbkdf1.h
- pbkdf1.c
- pc_profiler.h
- pc_profiler.c
- static_profiler.h
- static_profiler.c
- ticktime.h
- ticktime.c
- semihost_hardfault.c


Call Tree:

\-- main
    |-- BOARD_InitBootPins
    |-- BOARD_InitBootClocks
    |-- BOARD_InitBootPeripherals
    |-- init_ticktime
    |-- run_tests
    |-- time_pbkdf1
    |   \-- pbkdf1
    |       |-- ISHAReset
    |       |-- ISHAInput
    |       |   \-- ISHAProcessMessageBlock
    |       |-- ISHAResult
    |       |   \-- ISHAPadMessage
    |-- static_profile_on
    |-- time_pbkdf1
    |-- static_profile_off
    |-- print_static_profiler_summary
    |-- pc_profile_on
    |-- time_pbkdf1
    |-- pc_profile_off
    \-- print_pc_profiler_summary

Changes Made to optimize the Project:
- ISHAProcessMessageBlock():
  - Removed for-loop used to load W array with Mblock Bytes.
  - Used pointer Mblock to store ctx->Mblock array pointer, this helps reduce the
    re-referencing Mblock pointer via ctx.
  - Used a single variable W which is updated in the loop instead of W[] array.
  - Used Builtin bswap function to to store message bytes to variable W.
  - Over all gained speed and reduced code space
- ISHAPadMessage():
  - Reduced code space by removing code duplication in else block.
- ISHAResult():
  - Unrolled the for-loop to ouput the derived key gained speed as there is for-loop
    overhead.
- ISHAInput():
  - Modified the loop to copy more bytes in lesser iterations.
  - Used Pointer for ctx->Mblock which reduced re-referencing ctx everytime message bytes
    is copied.
  - Overall gained sginificant speed improvements.
- ISHAReset():
 - The function is written in Assembly in ISHAReset.s file and is integrated to work with
   other source files.

- pbkdf1.c doesn't use malloc any more. 
- main.c and pbkdf1.c doesn't require string.h library as strlen function used in main
  is replaced my a function defined in main.c and pbkdf1.c doesn't require strcpy any
  more.


Static Profile for function call before optimization:
ISHAProcessMessageBlock calls: 4096
ISHAPadMessage calls: 4096
ISHAReset calls: 4096
ISHAInput calls: 4097
ISHAResult calls: 4096

PC Profile for function call before optimization:
ISHAProcessMessageBlock calls: 5120
ISHAPadMessage calls: 2048
ISHAReset calls: 1025
ISHAInput calls: 5120
ISHAResult calls: 2048

Action	        Console	      Build	    .Text (code)	.Data	  .bss	    Total (dec)	    Total (hex)						
Baseline (-O0)	    UART	    DEBUG	    16328	          8	      2576	    18912	            49E0
Baseline (-O3)	    UART	    DEBUG	    13044	          8	      2576	    15628	            3D0C
Baseline (-Os)	    UART	    DEBUG	    10184	          8	      2576	    12768	            31E0
Baseline (-O0)	    UART	    RELEASE  	15276	          8	      2576	    17860	            45C4
Baseline (-O3)	    UART	    RELEASE	  12292	          8	      2576	    14876	            3A1C
Baseline (-Os)	    UART	    RELEASE	  9340	          8	      2576	    11924	            2E94

Static Profile for function call after optimization:
ISHAProcessMessageBlock calls: 4096
ISHAPadMessage calls: 4096
ISHAReset calls: 4096
ISHAInput calls: 4097
ISHAResult calls: 4096

PC Profile for function call after optimization: (Adjust Scale factor in ticktime.c but keep it below 4. This works in -O3)
ISHAProcessMessageBlock calls: 2344
ISHAPadMessage calls: 1363
ISHAReset calls: 195
ISHAInput calls: 0
ISHAResult calls: 584

Action	        Console	      Build	    .Text (code)	.Data	  .bss	    Total (dec)	    Total (hex)						
Baseline (-O0)	    UART	    DEBUG	    17120	          12	      2624	    18912	            4D2C
Baseline (-O3)	    UART	    DEBUG	    13828	          12	    2624	    16464	            4050
Baseline (-O0)	    UART	    RELEASE  	14704	          8	      2572	    17284	            4384
Baseline (-O3)	    UART	    RELEASE	  13460	          8	      2572	    16040	            3EA8
