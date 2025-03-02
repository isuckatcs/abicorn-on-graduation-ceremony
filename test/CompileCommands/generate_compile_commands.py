import pathlib

path = pathlib.Path(__file__).parent.resolve()

for v, a in [["old", "-std=c++98"], ["new", "-std=c++11"]]:
    dir = path.joinpath("build_" + v)
    dir.mkdir(exist_ok=True)

    with open(dir.joinpath("compile_commands.json"), "w") as file:
        file.write('[{')
        file.write('"directory": "{0}", "command": "clang++ ../{1}.cpp {2}", "file": "../{3}.cpp"'.format(dir, v, a, v))
        file.write('}]')
