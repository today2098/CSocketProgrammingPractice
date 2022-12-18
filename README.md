# CSocketProgrammingPractice

C言語でのソケットプログラミングを勉強した際の成果物



## Table of Contents

- [Requirement](#requirement)
- [Usage](#usage)
   - [Install](#install)
   - [Build](#build)
- [Description](#description)
- [Reference](#reference)



<a id="requirement"></a>
## Requirement

- Ubuntu 20.04 (WSL1)
- gcc 9.4.0
- cmake 3.16.3



<a id="usage"></a>
## Usage

<a id="install"></a>
### Install

```bash
$ cd your/workspace/path
$ git clone https://github.com/today2098/CSocketProgrammingPractice.git
```



<a id="build"></a>
### Build

```bash
$ cd CSocketProgrammingPractice
$ ./build.sh
```



<a id="description"></a>
## Description

|Project|Description|Protocol|
|---|---|:---:|
|SimpleTcp|サーバからクライアントへメッセージを送信する．|TCP|
|SimpleFT|クライアントからサーバへファイルデータを送信する．|TCP|
|SimpleUdp|メッセージを送受信する．|UDP|
|UdpEcho|サーバがクライアントから送られるメッセージをそのまま返信する．|UDP|
|Multicast|メッセージをマルチキャストする．|UDP|
|MulticastFT|ファイルデータをマルチキャストする．|UDP|



<a id="reference"></a>
## Reference

1. Michael J.D., Kenneth L.C.著，小高 知宏監訳『TCP/IPソケットプログラミング』オーム社 (2003)．
1. 富永 和人，権藤 克彦著『例解UNIX/Linuxプログラミング教室』オーム社 (2018)．
