# 3D-renderer с нуля

Целью данного проекта является реализовать 3D-renderer с нуля. В его имплементации применяются следующие сторонние библиотеки: `SFML` и `Eigen`. При помощи первой из них происходит отображение пикселей на экран монитора, а при помощи второй &mdash; работа с матрицами. Разработка происходит с использованием компилятора `g++ 10.3.1` и стандарта `C++20`.

Основным классом, который реализует обёртку вокруг остальных, является `Application`. Он позволяет настроить мир, камеру и окно, куда будут проецироваться объекты из мира. После того как сцена настроена, запускается один из режимов: интерактивный или динамический. Первый из них позволяет перемещать и вращать камеру при помощи клавиатуры, а второй &mdash; вращает камеру по окружности вокруг центра, этот режим просто создаёт динамическую картинку, чтобы не двигать камеру самому. Они запускаются при помощи методов `RunInteractiveScene` и `RunRotatingScene` соответственно. Ниже приведён пример с вращающейся сценой.

<p align="center">
  <img src="https://user-images.githubusercontent.com/80764786/119265527-c6186f00-bbef-11eb-85be-dc9f82347f20.gif"/>
</p>

### Инструкция

В папке `examples` корневой директории располагаются три исходника, которые демонстрируют примеры использования библиотеки. Чтобы собрать проект на операционной системе `Linux`, нужно выполнить в терминале следующую последовательность команд:

```bash
git clone --recurse-submodules https://github.com/stabmind/3D-renderer.git
mkdir 3D-renderer/build
cd 3D-renderer/build
cmake .. -D PATH=path-to-src -D CMAKE_BUILD_TYPE=Release
make
```

где `path-to-src` может быть, например, `examples/interactive_scene_example.cpp`.

Сборка может потребовать установки следующих библиотек: freetype, x11, xrandr, udev, opengl, flac, ogg, vorbis, vorbisenc, vorbisfile, openal, pthread.
