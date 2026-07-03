<div align="center">
  <img src="https://github.com/SeppoPakonen/VerseDesk/blob/main/docs/logo.png?raw=true" alt="VerseDesk Logo" width="250">
</div>

# VerseDesk

A professional-grade desktop application for structured songwriting and AI-assisted lyric editing, built with C++ and the Ultimate++ framework.

## Overview

VerseDesk is a sophisticated tool designed for songwriters, producers, and AI researchers who need fine-grained control over lyric composition, AI-generated alternatives, and iterative refinement. It combines traditional songwriting workflows with modern AI capabilities, enabling seamless integration of machine-generated content while maintaining artistic integrity.

**Core Use Cases:**
- Interactive lyric editing with metadata management
- AI-powered lyric generation and refinement
- Multi-alternative comparison and selection workflows
- Real-time evaluation metrics (singability, punchiness, rhyme quality, etc.)
- Version history and control tracking

## Key Features

### Structured Editing
- **Lyric Management**: Line-by-line editing with metadata (colors, ratings, rhyme groups)
- **Metadata Dock**: Song title, artist, key, tempo, and style attributes
- **Part Attributes**: Organize sections into verses, choruses, bridges with structural control
- **Line Evaluator**: Real-time feedback on singability, flow, and quality metrics

### AI Integration
- **Total Conversion Workflow**: Clipboard-based TOON/JSON import and export
- **Comparison Dialog**: Side-by-side alternative review with inline editing
- **Batch Processing**: Accept/reject alternatives with automatic metadata propagation
- **Schema Validation**: Built-in TOON parser with robust error recovery

### Advanced Features
- **Recent Files**: Quick access to previously edited projects
- **Rules Engine**: Customizable songwriting rules (JSON-based, multi-language support)
- **Version History**: Track and review editing sessions
- **Dock Layout**: Flexible, resizable workspace organization

<div align="center">
  <img src="https://github.com/SeppoPakonen/VerseDesk/blob/main/docs/screenshot_v0_1.jpg?raw=true" alt="VerseDesk Interface" width="450">
</div>

## Technical Stack

- **Language**: C++17
- **Framework**: Ultimate++ (U++) – custom fork at [OuluBSD/ai-upp](https://github.com/OuluBSD/ai-upp)
- **GUI**: U++ CtrlLib with Docking support
- **Image Support**: plugin/png for logo rendering
- **Data Format**: TOON (Term-Oriented Object Notation) for structured data exchange
- **Platform**: Windows (MSVS 2026 x64)

## Building

### Prerequisites
1. **Visual Studio Community Edition 2026** (or later)
   - Download from [visualstudio.microsoft.com](https://visualstudio.microsoft.com)
   - Install with C++ development tools (MSVC compiler, Windows SDK)

2. **ai-upp fork** (Ultimate++ with extensions)
   ```bash
   cd ..
   git clone https://github.com/OuluBSD/ai-upp.git
   ```

3. **Bootstrap the build system** (first time only)
   ```powershell
   cd ../ai-upp
   powershell -ExecutionPolicy Bypass -File stdsrc/build/bootstrap.ps1
   ```

### Build Command
   
Once prerequisites are installed:
```powershell
cd VerseDesk
../ai-upp/bin/build.exe -m MSVS26x64 --source-roots ".;../ai-upp" -j4 VerseDesk
```

The compiled executable will be in `bin/VerseDesk.exe`.

For detailed build instructions, see [COMPILING.md](COMPILING.md).

## Architecture

### Project Structure
```
src/VerseDesk/
├── MainWindow.cpp/h      – Main UI and event handling
├── SongModel.h          – Core data model for songs and metadata
├── TotalConversionDialog.cpp/h  – AI alternative comparison interface
├── DockViews.cpp/h      – Dockable UI panels (metadata, evaluator, history)
├── MenuSetup.cpp        – Application menu configuration
├── ToolBarSetup.cpp     – Toolbar definitions
├── RuleModel.h          – Songwriting rules engine
├── AppState.h           – Session state management
└── rules/               – JSON rule definitions (EN, FI)
```

### Data Flow
1. **User Input** → MainWindow event handlers
2. **Song Model** → Centralized data structure (lyrics, metadata, ratings)
3. **AI Integration** → TOON/JSON clipboard exchange
4. **Comparison** → Alternative selection and merging
5. **Persistence** → File-based project storage

## Contributing

This project maintains a detailed development log in the `docs/` directory. Each numbered entry documents major features, bug fixes, and architectural decisions.

Recent work:
- TOON parser hardening and schema validation
- AI-assisted title generation for conversions
- Grid scroll position preservation
- Clipboard serialization improvements

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact & Community

Built with attention to both artistic and technical excellence. Designed for professionals in songwriting, music production, and AI-assisted content creation.

---

**VerseDesk** | Structured Songwriting for the AI Era
