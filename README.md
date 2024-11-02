<div align="left" style="position: relative;">
<img src="https://github.com/user-attachments/assets/ebcd2c79-ee57-4fcb-8e02-dafbed184c5f" align="right" width="30%" style="margin: -20px 0 0 20px;">
<h1>MadInjector GUI</h1>
</p>
<p align="left">
	<img src="https://img.shields.io/github/license/eli64s/readme-ai?style=default&logo=opensourceinitiative&logoColor=white&color=ce00ff" alt="license">
</p>
  
</p>
<p align="left">

</p>
</div>
<br clear="right">

##  Table of Contents

I. [ Overview](#-overview)
II. [ Features](#-features)
III. [ Project Structure](#-getting-started)
IV. [ Credits](#-Credits)

---

##  Overview

<text>❯ This repository only contains the Qt GUI code which has nothing to do with my MadInjector Library which can be found here https://github.com/rickmad11/MadInjector. 

Also Note this will be the only repository containing the compiled binaries for the Library and the GUI application 
so if your here to just use the Injector then this is the right place! 

The Injector is focused on x64 targets however some basic features do work on x86 as well. 

Also note this was only tested on Windows 11 Version 23H2 Build 22631.4317 however it should also work with older Version as well as with Windows 10, but I can not guarantee every feature working on it.

<img src="https://github.com/user-attachments/assets/3199c267-e470-4582-97df-293885416847" style="margin: -80px 0 0 80px;">

---

##  Features

<text>❯ 
**Supported Execution Methods:**

-> CreateRemoteThread x64/x86

❯ NtCreateThreadEx x64/x86

        -> Hide From Debugger

        -> Skip Thread Attach

        -> Start Address Spoof

❯ SetWindowsHook x64/x86

❯ ThreadHijacking x64/x86

❯ QueueUserAPC x64/x86

❯ KernelCallbackTable x64/x86

❯ VectoredExceptionHandler x64

❯ ThreadPool x64

        -> WorkerThreadEntry
        
        -> WorkInsert
        
        -> JobInsert
        
        -> Direct
        
        -> Wait
        
        -> Timer
        
        -> Io
        
        -> Alpc

❯ InstrumentationCallback x64 

❯ TLS Callback x64 


**Supported Injection Methods:**

❯ LoadLibrary

❯ LdrLoadDll

❯ LdrpLoadDll

❯ LdrpLoadDllInternal

❯ Manual Map x64 -> please view the picture for all of the options

**Additional Features:**

-> Handle Hijacker x64 -> System Process only (this is a optional flag)

-> Unlink Module Button x64 -> can be any module all the Injector needs is the Path to the already loaded Module

-> Unlink Module CheckBox x64 -> This unlinks the Module right after execution the Dll in the target

-> Unload Mapped Dll x64 -> has some flaws but only works if injected with my Manual Mapper

-> Save/Load/Reset Settings

##  Getting Started

###  Prerequisites

-**Windows 10 or Greater** 

###  Installation

**Simply Download the Zip file and run MadInjector.exe which requires UAC.
Note this is only a stub the real executable is inside the Release Folder however I highly recommend using the Stub since it hides all the ugly files**

##  License
This project is protected under the [MIT-LICENSE]([https://choosealicense.com/licenses](https://choosealicense.com/licenses/mit/)) License. For more details, refer to the [LICENSE]([https://choosealicense.com/licenses/](https://github.com/rickmad11/MadInjector-GUI/blob/master/LICENSE)) file.

## Credits
**ThreadPool Injection :** 

https://www.safebreach.com/blog/process-injection-using-windows-thread-pools/

https://urien.gitbook.io/diago-lima/a-deep-dive-into-exploiting-windows-thread-pools

https://www.youtube.com/watch?v=AvBO4f7blew

**General Projects/Websites that made this Project possible in the first place**

https://github.com/guided-hacking/GuidedHacking-Injector

https://www.debuginfo.com/articles/debuginfomatch.html

https://ntdoc.m417z.com/

https://github.com/paskalian/WID_LoadLibrary/tree/main

https://www.geoffchappell.com/

https://github.com/winsiderss/systeminformer

https://doxygen.reactos.org/

https://learn.microsoft.com/en-us/windows/

https://github.com/Deputation/instrumentation_callbacks

https://csandker.io/2022/05/24/Offensive-Windows-IPC-3-ALPC.html

---
