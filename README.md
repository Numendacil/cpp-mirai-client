# cpp-mirai-client
<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![UnitTest Status][unittest-shield]][unittest-url]
[![FOSSA Status][fossa-shield]][fossa-url]
[![AGPLv3.0 License][license-shield]][license-url]
[![Project Version][projversion-shield]][projversion-url]
[![C++ Version][cppversion-shield]][cppversion-url]

## 关于本项目
Currently working in progress.

cpp-mirai-client 是一个基于 [*mirai*](https://github.com/mamoe/mirai) 与
 [*mirai-api-http*](https://github.com/project-mirai/mirai-api-http) 的跨平台C++机器人框架。

本项目在一定程度上参考了 [cyanray/mirai-cpp](https://github.com/cyanray/mirai-cpp) 项目，您也可以考虑使用该项目进行C++
QQ机器人的开发

## 安装要求
本项目使用CMake进行项目构建，要求CMake版本3.16以上。编译器需要支持C++17标准，目前已在最新版本的GCC、Clang与MSVC
编译器下测试通过。

使用cpp-mirai-client只需要将整个仓库克隆或下载到你的项目子目录中，并在项目的CMakeList.txt中添加
```CMake
add_subdirectory(/path/to/cpp-mirai-client)
set(MIRAI_BUILD_UNIT_TESTS OFF CACHE BOOL "Build UnitTests" FORCE)
target_link_libraries(your_target_name PRIVATE cpp-mirai-client::cppmirai)
```
即可。

也可以通过 cppmirai-template 直接生成一个基础的程序框架（TBD）

## 项目依赖
cpp-mirai-client 使用了以下第三方项目，可在 `external/` 文件夹内找到

1. [**machinezone/IXWebSocket**](https://github.com/machinezone/IXWebSocket) websocket and http client and server library, with TLS support and very few dependencies 
2. [**yhirose/cpp-httplib**](https://github.com/yhirose/cpp-httplib) A C++ header-only HTTP/HTTPS server and client library
3. [**nlohmann/json**](https://github.com/nlohmann/json) JSON for Modern C++ 
4. [**google/googletest**](https://github.com/google/googletest) GoogleTest - Google Testing and Mocking Framework

其中 [google/googletest](https://github.com/google/googletest) 仅用于测试，实际使用时可以不用安装。
这些项目都已经以submodule的形式被包含在了 `external/` 目录下，可以通过 `git clone --recursive` 来拉取对应的模块并本地编译链接。
你也可以单独安装这些第三方库，cpp-mirai-client会自动使用 `find_package` 来找到对应的文件。所有依赖均为私有依赖，仅用于编译，使用cpp-mirai-client
时不需要链接这些第三方库。

本项目的文档生成使用了 [Doxygen](https://doxygen.org/index.html) 与 [jothepro/doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css)。

## TODO
- Docs and example usages
- Project templates for creating a simple QQBot
- Benchmark and performance tests

<span class="next_section_button">

Read Next: [快速入门](docs/usage/_0_quickstart.md)
</span>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[unittest-shield]: https://img.shields.io/github/actions/workflow/status/numendacil/cpp-mirai-client/UnitTest.yml?branch=master&style=flat&logo=github
[unittest-url]: https://github.com/numendacil/cpp-mirai-client
[license-shield]: https://img.shields.io/github/license/numendacil/cpp-mirai-client?style=flat
[license-url]: https://github.com/numendacil/cpp-mirai-client/blob/master/LICENSE
[projversion-shield]: https://img.shields.io/badge/version-2.6.2-blue.svg?style=flat
[projversion-url]: https://github.com/numendacil/cpp-mirai-client
[cppversion-shield]: https://img.shields.io/badge/c++-17-yellow.svg?style=flat
[cppversion-url]: https://github.com/numendacil/cpp-mirai-client
[fossa-shield]: https://app.fossa.com/api/projects/git%2Bgithub.com%2FNumendacil%2Fcpp-mirai-client.svg?type=shield
[fossa-url]: https://app.fossa.com/projects/git%2Bgithub.com%2FNumendacil%2Fcpp-mirai-client?ref=badge_shield