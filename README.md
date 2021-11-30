# ILisp
ILisp is a toy Lisp REPL with C++ Runtime.

ILisp can act as a simple calculator, equipped with "if" "define" "begin" keyword.

## Requirements
### Linux/macOS
[ediline](https://github.com/troglobit/editline) for cli interaction.

[CMake](https://cmake.org/) or [XMake](xmake.io) for building the project.

### Windows
[CMake](https://cmake.org/) or [XMake](xmake.io) for building the project.

## Build
### CMake
```shell
git clone https://github.com/kabu1204/ILisp.git
cd ILisp
cmake . -B $path_to_build -DCMAKE_BUILD_TYPE=$mode    # mode = Release|Debug|RelWithDebInfo
cmake --build $path_to_build --target ILisp
$path_to_build/ILisp
```

### XMake
```shell
git clone https://github.com/kabu1204/ILisp.git
cd ILisp
xmake f -m $mode   # mode = release|debug|releasedbg
xmake
xmake run ilisp
```

## Examples
![example0](pics/example0.png)

![example1](pics/example1.png)
$$
\begin{align}
&sin(\frac{\pi}{2})\\
&\left\{\begin{aligned}
&\log_{10}{100}\quad &if\ 1>2\\
&\log_{2}{8}\quad &otherwise
\end{aligned}\right.\\
&\sqrt{sin(\frac{\pi}{2})+|cos(\pi)|}
\end{align}
$$

## REF
[SICP](https://mitpress.mit.edu/sites/default/files/sicp/full-text/book/book.html)

[Lispy](http://norvig.com/lispy.html)