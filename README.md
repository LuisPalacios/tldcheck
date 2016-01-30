# IANA TLD Check

This project downloads the Top Level Domains from IANA text file, converts it to UTF-8, and creates a header file that can be used in C++ applications. I have included a small C++ program that allows you test it.

The project consists of two main components:

**Python Component (`gettld.py`):**

- Downloads the latest TLD list from IANA
- Adds "custom" tld domains, like "local", check the source.
- Converts Punycode to UTF-8 and lowercase normalization
- Generates optimized C++ header file with `std::set` for fast lookups

**C++ Component:**

- URI Parser and Validator (`src/uri.h/cpp`): RFC 3986 compliant small uri parser
- TLD Validator: Validates domains against the IANA TLD list

**Python Environment Setup:**

```shell
# Create virtual environment (first time only)
python -m venv myenv

# Activate environment:
.\myenv\Scripts\Activate.ps1    # Windows PowerShell
source myenv/Scripts/activate   # macOS/Linux/WSL

# Install dependencies
pip install requests idna

# Optional if you want to freeze requirements and use
# them in the future:
# pip freeze > requirements.txt
# pip install -r requirements.txt

# Download and process TLD list
python gettld.py
```

**C++ Build Instructions:**

Tested on Windows, macOS and Linux with with **Clang 17**, **CMake** 3.10 or higher and **Ninja** build system (recommended), so you need to install LLVM@17, cmake and ninja in Windows, or macOS or Linux.

```bash
# Windows/Linux/macOS: Prepare and Build
cmake -S . -B build -G Ninja
cmake --build build
```

## Usage

```bash
# Windows/Linux/MacOS
python gettld.py

# Windows
.\build\tldcheck.exe "https://www.google.com"

# Linux/macOS
./build/tldcheck "https://www.google.com"
```

## License

This project is licensed under the terms specified in the [LICENSE](LICENSE) file.
