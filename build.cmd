Rem cl ../../libs/fmt/src/format.cc /I ../../libs/fmt/include /Fo"bin/" /c /EHsc
cl src/main.cpp /Fo"bin/" /EHsc /I../../libs/fmt/include bin/format.obj /Zi /std:c++17
