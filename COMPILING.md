# Building VerseDesk

VerseDesk is a U++ desktop application for editing song projects. It requires the Ultimate++ framework (U++) to build.

## Prerequisites

### 1. Clone the U++ Fork

VerseDesk requires a custom fork of Ultimate++ (ai-upp):

```bash
cd ..
git clone https://github.com/OuluBSD/ai-upp.git
```

The `ai-upp` directory should be a sibling of the `VerseDesk` directory:
```
Dev/
  ├── VerseDesk/
  └── ai-upp/
```

### 2. Bootstrap the Build System (First Time Only)

If `../ai-upp/bin/build.exe` does not exist, you need to run the bootstrap script:

```powershell
cd ../ai-upp
powershell -ExecutionPolicy Bypass -File stdsrc/build/bootstrap.ps1
```

This will compile the U++ build tools and create the `bin/build.exe` executable.

## Building VerseDesk

Once the U++ build system is set up, build VerseDesk:

```powershell
cd VerseDesk
../ai-upp/bin/build.exe -m MSVS26x64 --source-roots ".;../ai-upp" -j4 VerseDesk
```

### Build Options

- `-m MSVS26x64`: Use Visual Studio 2026 (64-bit) compiler
- `--source-roots ".;../ai-upp"`: Source directories for build system
- `-j4`: Number of parallel build jobs (adjust based on your CPU cores; use 2-4 for safe builds)

### Output

The compiled executable will be in:
```
bin/VerseDesk.exe
```

## Troubleshooting

### "Package VerseDesk does not exist"

If you get this error, ensure:
1. The `VerseDesk.upp` file exists in `src/VerseDesk/`
2. The filename matches the package name exactly
3. The `../ai-upp/bin/build.exe` is up to date (regenerate with bootstrap if needed)

### Build Fails

1. Make sure Visual Studio 2026 is installed
2. Verify the `ai-upp` repository is fully cloned
3. Try removing the `bin/` directory and rebuilding from scratch

### Running the Application

After a successful build:
```powershell
.\bin\VerseDesk.exe
```

## Development Notes

- The U++ project file is `src/VerseDesk/VerseDesk.upp`
- Source files are in `src/VerseDesk/`
- Additional rules are in `src/VerseDesk/rules/`
- Documentation is in `docs/`
