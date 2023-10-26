# HolySheet App


<!-- PROJECT SHIELDS -->

[![Contributors][contributors-shield]][contributors-url][![Forks][forks-shield]][forks-url][![Stargazers][stars-shield]][stars-url][![Issues][issues-shield]][issues-url][![MIT License][license-shield]][license-url]

<!-- PROJECT LOGO -->
<br />

<p align="center">
  <a href="https://github.com/SSRVodka/HolySheet">
    <img src="imgs/disp.svg" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">HolySheet App v0.1</h3>
  <p align="center">
    A cross platform application for managing datasheet - Written in Qt5 / C++.
    <br />
    <a href="https://github.com/SSRVodka/HolySheet"><strong>Have a look at the source code ?</strong></a>
    <br />
    <br />
    <a href="https://github.com/SSRVodka/HolySheet/releases">App Releases</a>
    ·
    <a href="https://github.com/SSRVodka/HolySheet/issues">Report a Bug</a>
    ·
    <a href="https://github.com/SSRVodka/HolySheet/issues">Issure a New Feature</a>
  </p>

</p>

## Contents
- [Quick Start](#Quick Start)
  - [Installation for Windows](#Installation for Windows)
  - [Installation for Linux](#Installation for Linux)
- [Features](#Features)
- [TODO list (Maybe)](#TODO list (Maybe))
- [Project Structure](#Project Structure)
- [Development Framework](#Development Framework)
- [Version Control](#Version Control)
- [Contributors](#Contributors)
- [Co-development](#Co-development)
- [中文 README](#目录)



## Quick Start

We provide supports for both Linux and Windows.

Please enter the `release` tab and find the zip of the binary program of the latest adapted version. 

The program does not provide an installation wizard, so you can unzip the program into a suitable directory after downloading.

### Installation for Windows

- Operating system requirements: Windows 10+ with more than 2 GB of RAM.
- Unzip the program and double-click it to run it. It is recommended to create a shortcut to `HolySheet.exe` on your desktop.

### Installation for Linux

- Adaptation to most Linux distributions, for example Ubuntu, version should not be lower than 18. RAM should not be lower than 2 GB.
- Linux operating systems **need to have a graphical interface installed to present the application**, otherwise errors will occur.
- After unpacking, run `install.sh` on the command line, type "Y" and enter to install.
- After the installation is complete, the icon will be displayed in the menu bar.



## Features

This program was developed to mimic Microsoft Excel, but this program is much smaller and installation free. It fulfills the following features:

- [x] **Save feature**: save with `*.hs` suffix, encrypt data without fear of leakage;

- [x] **Find feature**: case sensitive option, forward/backward find;

- [x] **Location feature**: check if user input is legal or not, do not allow to fill in if it is not legal;

- [x] **Paste feature**: paste unsupported data to alert users;

- [x] **Sorting feature**: design the sorting window to allow the user to sort the selected fields. The user can be provided with 3 sort keys for sorting, and the user can select the ascending and descending order of these 3 keys;

- [x] **Syntax Recognition feature**:
  
  - Formula Mode: When user inputs content starting with "=", the content after the equal sign is treated as a formula. See "**Formula feature**" for details;
  
  - Pure String Mode: Users can directly input content that does not start with "=" or any content that starts with the English single quote " ' ", i.e., pure string mode;
  
  - examples:
  
    ```
    LOVE-SJTU is pure string mode, which will display: LOVE-SJTU
    
    =(1+2)*9^0.5/2 is formula mode, you will see 4.5 in the entry.
    
    '=123 is pure string mode, which will display: =123
    ```
  
- [x] **Formula feature**:

  - **Capable of evaluating pure numeric expressions**. Includes `+-*/^` and parentheses. For example, entering `=1+2` in a cell displays `3`;

  - **Capable of calculating expressions containing cell codes**. For example, if the contents of cell `A1` are `12` and the contents of cell `B2` are `11`, then enter `=A1+B2` in any other cell to display `23`;

  - **Ability to cascade calculations and update data**. For example, the content of cell `A1` is `10`, the content of cell `B2` is `=A1+1` (displaying `11`), the content of cell `C3` is `=B2*2+A1` (displaying `32`); if the value of `A1` is changed to `1`, then `B2` is automatically changed to `2` and `C3` is automatically changed to `5`;

  - **Capable of handling some aggregate functions**. Implement the `SUM` sum function and the `AVG` average function with the following syntax:

    ```
    =AVG(<startPos>:<endPos>)
    =SUM(<startPos>:<endPos>)
    
    examples:
    The digits in the cell A1 ~ A9 are 1, 2, ..., 9, respectively.
    If we type contents below into A10:
    =SUM(A1:A9) will display 45
    =AVG(A1:A9) will display 5
    ```

  - **Capable of handling illegitimate expressions**. The user should be warned about illegal expressions and `####` should be displayed in the illegal position.

- [x] **Display feature**:

  - If the content is a pure number, then it is displayed vertically centered to the right in the cell;

  - If the content is a string, then  it is displayed centered vertically to the left;



## TODO list (Maybe)

- [ ] Multiple tabs for managing datasheet.
- [ ] File convertion & export.
- [ ] ......



## Project Structure

```
.
├── CMakeLists.txt			# Project Compilation Configuration
├── LICENSE
├── README.md
├── icon.rc					# File for icon
├── resources.qrc			# Qt config file for managing resources
|
├── main.cpp
├── imgs/
├── include/				# Headers
|	├── calc.h
|	├── Cell.h
|	├── findDialog.h
|	├── goToCellDialog.h
|	├── HolySheet.h
|	├── linStack.h
|	├── MainWindow.h
|	└── sortDialog.h
├── src/					# Sources
|	├── calc.cpp
|	├── Cell.cpp
|	├── findDialog.cpp
|	├── goToCellDialog.cpp
|	├── HolySheet.cpp
|	├── MainWindow.cpp
|	└── sortDialog.cpp
└── ui/						# Qt UI Designer files
	├── goToCellDialog.ui
	└── sortDialog.ui

```



## Development Framework

- [Qt 5](https://doc.qt.io/qt-5/index.html)



## Version Control

The project uses `Git` for versioning. You can see the currently available versions in the repository.



## Contributors

Only myself :sweat_drops: (for now).



## Co-development

Co-development is very welcome :heart_eyes: and you can follow the steps below:

1. Fork the Project
2. Create your Feature Branch (`git checkout -b featureBranch`)
3. Clone the repo
4. Commit your Changes && Push to the Branch
5. Open a Pull Request

Or you can just share some issues / ideas :boom: to help me better Thanks a lot :satisfied: !!!



---



## 目录

- [快速开始](#快速开始)
  - [Windows 安装](#Windows 安装)
  - [Linux 安装](#Linux 安装)
- [功能简述](#功能简述)
- [可能的 TODO list](#可能的 TODO list)
- [项目目录说明](#项目目录说明)
- [使用到的框架](#使用到的框架)
- [版本控制](#版本控制)
- [贡献者](#贡献者)
- [共同参与开发](#共同参与开发)



## 快速开始

本程序提供对 Linux 和 Windows 操作系统的支持。

请进入 release 页签，找到最近适配版本的二进制程序压缩包。程序不提供安装向导，所以请在下载后解压到合适的目录。

### Windows 安装

- 操作系统要求 Windows 10+，内存大于 2 GB.
- 解压后双击程序运行。建议自行在桌面创建指向 `HolySheet.exe` 的快捷方式.

### Linux 安装

- 适配大多数 Linux 发行版操作系统，以 Ubuntu 为例，版本不宜低于 18. 内存不宜低于 2 GB.
- Linux 操作系统**需要安装图形化界面以呈现应用**，否则会出现错误.
- 解压后，命令行运行 `install.sh`，输入 "Y" 并回车即可安装.
- 安装完成后，图标会显示在菜单栏.



## 功能简述

本程序模仿 Microsoft Excel 开发，但本程序更小巧、免安装。满足以下特性：

- [x] **保存功能**：使用 `*.hs` 后缀进行保存，加密数据，无惧泄露；

- [x] **查找功能**：大小写敏感选项、向前/向后查找；

- [x] **定位功能**：检查用户输入是否合法，不合法不允许填入；

- [x] **粘贴功能**：粘贴不支持的数据提醒用户；

- [x] **排序功能**：设计排序窗口，能够让用户对于所选的区域进行排序。可以为用户提供 3 个排序键的排序，用户可以选择这 3 个键的升序、降序情况；

- [x] **语法识别功能**：

  - 公式模式：用户以 “=” 开头输入内容时，等号后面的内容按照公式处理。详见 “公式特性”；

  - 纯字符串模式：用户可以直接输入不以 “=” 开头的内容，或者以英文单引号 “ ' ” 开头的任何内容，即为纯字符串模式；

  - 示例：

    ```
    LOVE-SJTU 是纯字符串模式，应该显示为 LOVE-SJTU
    
    =(1+2)*9^0.5/2 是公式模式，应该显示为 4.5
    
    '=123 是纯字符串模式，应该显示为 =123
    ```

- [x] 公式特性：

  - **能够计算纯数字表达式**。包含 `+-*/^` 和括号。例如单元格中输入 `=1+2` 显示 `3`；

  - **能够计算含有单元格代号的表达式**。例如 `A1` 单元格的内容是 `12`，`B2` 单元格的内容是 `11`，那么任意其他单元格中输入 `=A1+B2` 显示 `23`；

  - **能够级联计算和更新数据**。例如 `A1` 单元格的内容是 `10`，`B2` 单元格的内容是 `=A1+1` （显示 `11`），`C3` 单元格的内容是 `=B2*2+A1`（显示 `32`）；如果将 `A1` 的值更改为 `1`，那么 `B2` 自动变更为 `2`，`C3` 自动变更为 `5`；

  - **能够处理一些聚合函数**。实现 `SUM` 求和函数和 `AVG` 求平均值函数，语法如下：

    ```
    =AVG(<startPos>:<endPos>)
    =SUM(<startPos>:<endPos>)
    
    示例：
    A1 ~ A9 的单元格的数据分别为：1, 2, ..., 9
    那么在 A10 单元格中输入：
    =SUM(A1:A9) 显示 45
    =AVG(A1:A9) 显示 5
    ```

  - **能够处理不合法的表达式**。对于不合法的表达式应该警告用户，并在不合法的位置上显示 `####`。

- [x] 显示特性：

  - 如果内容是纯数字，那么在单元格中靠右垂直居中对齐显示；

  - 如果内容是字符串，那么在单元格中靠左垂直居中对齐显示；



## 可能的 TODO list

- [ ] 多页签数据表（tab - datasheet）；
- [ ] 文件类型转换 和 导出；
- [ ] ……



## 项目目录说明

```
.
├── CMakeLists.txt			# 项目配置
├── LICENSE
├── README.md
├── icon.rc					# 图标资源
├── resources.qrc			# Qt 全局资源配置文件
|
├── main.cpp				# 主函数入口
├── imgs/					# 程序资源图
├── include/				# 程序头文件
|	├── calc.h
|	├── Cell.h
|	├── findDialog.h
|	├── goToCellDialog.h
|	├── HolySheet.h
|	├── linStack.h
|	├── MainWindow.h
|	└── sortDialog.h
├── src/					# 程序源文件
|	├── calc.cpp
|	├── Cell.cpp
|	├── findDialog.cpp
|	├── goToCellDialog.cpp
|	├── HolySheet.cpp
|	├── MainWindow.cpp
|	└── sortDialog.cpp
└── ui/						# Qt UI 设计师文件
	├── goToCellDialog.ui
	└── sortDialog.ui

```



## 使用到的框架

- [Qt 5](https://doc.qt.io/qt-5/index.html)



## 版本控制

该项目使用Git进行版本管理。您可以在repository参看当前可用版本。



## 贡献者

（当前仅有本人



## 共同参与开发 

非常欢迎共同开发，您可以按照下面的步骤：

1. Fork 这个项目；
2. 将项目 Clone 至本地：`git clone https://github.com/SSRVodka/HolySheet.git`;
3. 创建您要加入的特性分支：`git checkout -b featureBranch`;
4. Commit & Push 到您的仓库中；
5. 在这个项目中提出 pull request。

您也可以仅仅提出一些建议。非常感谢 :satisfied: ！！



<!-- links -->

[your-project-path]:SSRVodka/HolySheet
[contributors-shield]: https://img.shields.io/github/contributors/SSRVodka/HolySheet.svg?style=flat-square
[contributors-url]: https://github.com/SSRVodka/HolySheet/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/SSRVodka/HolySheet.svg?style=flat-square
[forks-url]: https://github.com/SSRVodka/HolySheet/network/members
[stars-shield]: https://img.shields.io/github/stars/SSRVodka/HolySheet.svg?style=flat-square
[stars-url]: https://github.com/SSRVodka/HolySheet/stargazers
[issues-shield]: https://img.shields.io/github/issues/SSRVodka/HolySheet.svg?style=flat-square
[issues-url]: https://img.shields.io/github/issues/SSRVodka/HolySheet.svg
[license-shield]: https://img.shields.io/github/license/SSRVodka/HolySheet.svg?style=flat-square
[license-url]: https://github.com/SSRVodka/HolySheet/blob/master/LICENSE
