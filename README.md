# 📊 Exam Results Analyzer

> Desktop application for analyzing exam results with statistical calculations and data visualization

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![VCL](https://img.shields.io/badge/VCL-ED1F35?style=for-the-badge&logo=embarcadero&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

## 📋 About The Project

**Exam Results Analyzer** is a desktop application built with C++ and VCL (Visual Component Library) for automating the management and analysis of exam results across student groups. The application provides comprehensive statistical analysis, data visualization through histograms, and file management capabilities.

### Key Features

✅ **Data Management**
- Store exam results in a structured grid (groups × exams)
- Input validation with error handling
- Binary file format for save/load operations

✅ **Statistical Analysis**
- Calculate average scores per group and exam
- Compute grade distribution percentages
- Generate overall stream statistics

✅ **Visualization**
- Interactive histograms using TeeChart
- Color-coded grade distribution (2-5 scale)
- Visual representation of statistical data

✅ **File Operations**
- Save/load data in binary format
- Export text reports
- Export graphical reports as images

## 🛠️ Built With

- **Language**: C++
- **Framework**: VCL (Visual Component Library)
- **IDE**: Embarcadero C++ Builder
- **Charting**: TeeChart
- **Standard Library**: STL (Standard Template Library)

## 🎯 Project Structure

```
exam-results-analyzer/
├── Project1.cpp    # Main application entry point
├── main.h                        # Form header with component declarations
├── main.cpp                      # Main logic and implementation
└── README.md
```

### Core Components

| Component | Purpose |
|-----------|---------|
| `TStringGrid` | Data storage (groups × exams table) |
| `TChart` | Histogram visualization |
| `TBarSeries` | Bar chart data series |
| `TMemo` | Text statistics output |
| `TComboBox` | Group and exam selection |
| `TMainMenu` | Application menu system |
| `TFileOpenDialog` / `TFileSaveDialog` | File operations |

## 💡 How It Works

### Data Storage Format

Each cell in the grid contains a comma-separated string of 4 numbers representing grade counts:
```
2,3,5,7
```
Where:
- First number: count of grade "2" (fail)
- Second number: count of grade "3" (satisfactory)
- Third number: count of grade "4" (good)
- Fourth number: count of grade "5" (excellent)

### Statistical Calculations

**Average Score Formula:**
```
Average = (2×n₂ + 3×n₃ + 4×n₄ + 5×n₅) / (n₂ + n₃ + n₄ + n₅)
```

**Computed Metrics:**
- Average score per exam for each group
- Percentage distribution of grades
- Average score across all exams per group
- Overall stream statistics

### Histogram Visualization

The `DrawHistogram()` method generates color-coded bar charts:
- 🔴 Red: Grade "2"
- 🔵 Blue: Grade "3"
- 🟢 Green: Grade "4"
- 🟡 Yellow: Grade "5"

## 🚀 Features Implemented

### Menu System

**File Menu**
- Save
- Save As
- Open
- Exit

**Edit Menu**
- Clear table
- Add/Remove group
- Add/Remove exam

**Results Menu**
- Generate statistics report

### Error Handling

Robust exception handling for:
- File I/O errors
- Invalid data input
- Table size mismatches
- Corrupted data validation

## 📊 Technical Highlights

- **Object-Oriented Design**: Clean separation of UI and business logic
- **STL Containers**: Efficient data processing using `std::vector`
- **Input Validation**: Real-time validation preventing invalid entries
- **Exception Safety**: Comprehensive try-catch blocks for error handling
- **Binary Serialization**: Custom binary format for data persistence

## 🎓 Learning Outcomes

This project demonstrates practical application of:
- GUI application development with VCL
- Statistical algorithms implementation
- Data visualization techniques
- File I/O operations
- Exception handling patterns
- STL container usage
- Event-driven programming

## 🔮 Future Enhancements

Potential improvements identified:
- [ ] Automatic table resizing on file load
- [ ] File corruption detection
- [ ] Student name storage
- [ ] Excel/CSV export functionality
- [ ] Database integration
- [ ] Multi-language support

## 📝 License

This project was developed as a coursework assignment at MADI (Moscow Automobile and Road Construction University).

## 👤 Author

**Arina Minko**
- Email: arinaminko10@gmail.com
- University: MADI
- Program: Computer Science and Engineering

---

⭐ If you found this project interesting, please consider giving it a star!
