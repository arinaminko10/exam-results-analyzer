#include <vcl.h>
#pragma hdrstop
#include "main.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <map>
#include <algorithm>
#pragma hdrstop

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
	ComboBoxGroup->OnChange = ComboBoxGroupChange;
	ComboBoxExam->OnChange = ComboBoxExamChange;

	UpdateHeaders();

    TMenuItem *FileMenu = new TMenuItem(MainMenu);
	FileMenu->Caption = L"& Файл";
    MainMenu->Items->Add(FileMenu);

	TMenuItem *SaveItem = new TMenuItem(FileMenu);
	SaveItem->Caption = L"&Сохранить";
	SaveItem->ShortCut = TextToShortCut("Ctrl+S");
	SaveItem->OnClick = MenuItemClick;
	FileMenu->Add(SaveItem);

	TMenuItem *SaveAsItem = new TMenuItem(FileMenu);
	SaveAsItem->Caption = L"&Сохранить как...";
	SaveAsItem->ShortCut = TextToShortCut("Ctrl+Shift+S");
	SaveAsItem->OnClick = MenuItemClick;
	FileMenu->Add(SaveAsItem);

	TMenuItem *OpenItem = new TMenuItem(FileMenu);
	OpenItem->Caption = L"&Открыть...";
	OpenItem->ShortCut = TextToShortCut("Ctrl+O");
	OpenItem->OnClick = MenuItemClick;
	FileMenu->Add(OpenItem);

	TMenuItem *ExitItem = new TMenuItem(FileMenu);
	ExitItem->Caption = L"&Выход";
	ExitItem->ShortCut = TextToShortCut("Alt+F4");
	ExitItem->OnClick = MenuItemClick;
    FileMenu->Add(ExitItem);

    TMenuItem *EditMenu = new TMenuItem(MainMenu);
	EditMenu->Caption = L"&Редактирование";
    MainMenu->Items->Add(EditMenu);

    TMenuItem *CleanItem = new TMenuItem(MainMenu);
	CleanItem->Caption = L"&Очистить таблицу";
	CleanItem->ShortCut = TextToShortCut("Ctrl+Shift+C");
	CleanItem->OnClick = MenuItemClick;
	EditMenu->Add(CleanItem);

	TMenuItem *AddGroupItem = new TMenuItem(EditMenu);
	AddGroupItem->Caption = L"&Добавить группу";
	AddGroupItem->ShortCut = TextToShortCut("Ctrl+G");
	AddGroupItem->OnClick = MenuItemClick;
	EditMenu->Add(AddGroupItem);

	TMenuItem *DelGroupItem = new TMenuItem(EditMenu);
	DelGroupItem->Caption = L"&Удалить группу";
	DelGroupItem->ShortCut = TextToShortCut("Shift+G");
	DelGroupItem->OnClick = MenuItemClick;
	EditMenu->Add(DelGroupItem);

	TMenuItem *AddExamItem = new TMenuItem(EditMenu);
	AddExamItem->Caption = L"&Добавить экзамен";
	AddExamItem->ShortCut = TextToShortCut("Ctrl+E");
	AddExamItem->OnClick = MenuItemClick;
	EditMenu->Add(AddExamItem);

	TMenuItem *DelExamItem = new TMenuItem(EditMenu);
	DelExamItem->Caption = L"&Удалить экзамен";
	DelExamItem->ShortCut = TextToShortCut("Shift+E");
	DelExamItem->OnClick = MenuItemClick;
	EditMenu->Add(DelExamItem);

    TMenuItem *ResultMenu = new TMenuItem(MainMenu);
	ResultMenu->Caption = L"&Результат";
    MainMenu->Items->Add(ResultMenu);

	TMenuItem *ShowItem = new TMenuItem(ResultMenu);
	ShowItem->Caption = L"&Вывести результат";
	ShowItem->ShortCut = TextToShortCut("Ctrl+R");
	ShowItem->OnClick = MenuItemClick;
	ResultMenu->Add(ShowItem);
}

// Обработчик событий для элементов меню
void __fastcall TForm1::MenuItemClick(TObject *Sender)
{
	TMenuItem *MenuItem = dynamic_cast<TMenuItem*>(Sender);
    if (MenuItem != nullptr)
    {

		if (MenuItem->Caption == L"&Сохранить")
		{
			SaveToBinaryFile();
		}
		else if (MenuItem->Caption == L"&Сохранить как...")
		{
			SaveAsToBinaryFile();
		}
		else if (MenuItem->Caption == L"&Открыть...")
		{
			LoadFromBinaryFile();
		}
		else if (MenuItem->Caption == L"&Выход")
		{
			Application->Terminate();
			Close();
        }
		else if (MenuItem->Caption == L"&Очистить таблицу")
		{
			ClearTable();
		}
		else if (MenuItem->Caption == L"&Добавить группу")
		{
			AddGroup();
		}
		else if (MenuItem->Caption == L"&Удалить группу")
		{
			DelGroup();
		}
		else if (MenuItem->Caption == L"&Добавить экзамен")
		{
			AddExam();
		}
		else if (MenuItem->Caption == L"&Удалить экзамен")
		{
			DelExam();
		}
		else if (MenuItem->Caption == L"&Вывести результат")
		{
			ShowStatistics();
		}
    }
}

// Обновление заголовков таблицы
void TForm1::UpdateHeaders()
{
	for (int col = 0; col < StringGrid->ColCount; col++) {
		StringGrid->Cells[col][0] = (col == 0) ? " " : "Экзамен " + IntToStr(col);
	}
	for (int row = 1; row < StringGrid->RowCount; row++) {
		StringGrid->Cells[0][row] = "Группа " + IntToStr(row);
	}
}

// Построение гистограммы
void TForm1::DrawHistogram(int groupIndex, int examIndex) {
	Chart->SeriesList->Clear();
	TBarSeries *barSeries = new TBarSeries(Chart);
	Chart->AddSeries(barSeries);

    Chart->Title->Text->Clear();
    Chart->Title->Text->Add("Гистограмма: Группа " + IntToStr(groupIndex + 1) + ", Экзамен " + IntToStr(examIndex + 1));
    Chart->LeftAxis->Title->Caption = "Проценты";
    Chart->BottomAxis->Title->Caption = "Оценки";

    // Проверка индексов
    if (groupIndex < 0 || groupIndex >= StringGrid->RowCount - 1 || examIndex < 0 || examIndex >= StringGrid->ColCount - 1) {
        ShowMessage("Некорректные индексы группы или экзамена!");
        return;
    }

    // Парсинг данных
    auto cellContent = StringGrid->Cells[examIndex + 1][groupIndex + 1];
    auto counts = ParseGrades(cellContent);
    if (counts.empty()) {
        ShowMessage("Нет оценок для построения графика!");
        return;
    }

    // Вычисляем проценты
    int total = std::accumulate(counts.begin(), counts.end(), 0);
    if (total == 0) {
        ShowMessage("Нет данных для построения графика!");
        return;
    }

    std::vector<double> percentages(counts.size());
    std::transform(counts.begin(), counts.end(), percentages.begin(),
                   [total](int value) { return (double(value) / total) * 100.0; });

	barSeries->Add(percentages[0], " % - '2' ", clRed);
	barSeries->Add(percentages[1], " % - '3' ", clBlue);
	barSeries->Add(percentages[2], " % - '4' ", clGreen);
	barSeries->Add(percentages[3], " % - '5' ", clYellow);

	Chart->Repaint();
}

// Обновление ComboBox
void TForm1::UpdateComboBoxes()
{
    ComboBoxGroup->Items->Clear();
    ComboBoxExam->Items->Clear();

    for (int i = 1; i < StringGrid->RowCount; i++) {
        ComboBoxGroup->Items->Add("Группа " + IntToStr(i));
    }
    for (int j = 1; j < StringGrid->ColCount; j++) {
        ComboBoxExam->Items->Add("Экзамен " + IntToStr(j));
    }

    if (ComboBoxGroup->Items->Count > 0) ComboBoxGroup->ItemIndex = 0;
	if (ComboBoxExam->Items->Count > 0) ComboBoxExam->ItemIndex = 0;

	DrawHistogram(ComboBoxGroup->ItemIndex, ComboBoxExam->ItemIndex);
}

// При изменении выбора группы
void TForm1::ComboBoxGroupChange(TObject *Sender)
{
	int groupIndex = ComboBoxGroup->ItemIndex;
	int examIndex = ComboBoxExam->ItemIndex;

	if (groupIndex >= 0 && examIndex >= 0) {
		DrawHistogram(groupIndex, examIndex);
	}
}

// При изменении выбора экзамена
void TForm1::ComboBoxExamChange(TObject *Sender)
{
	int groupIndex = ComboBoxGroup->ItemIndex;
	int examIndex = ComboBoxExam->ItemIndex;

	if (groupIndex >= 0 && examIndex >= 0) {
		DrawHistogram(groupIndex, examIndex);
	}
}

// Добавление новой группы
void TForm1::AddGroup()
{
	int newRow = StringGrid->RowCount;
	StringGrid->RowCount++;
	StringGrid->Cells[0][newRow] = "Группа " + IntToStr(newRow);
	for (int col = 1; col < StringGrid->ColCount; col++) {
		StringGrid->Cells[col][newRow] = "";
	}
	ShowMessage("Добавлена новая группа!");
	UpdateComboBoxes();
}

// Удаление группы
void TForm1::DelGroup()
{
	int firstRow = StringGrid->Selection.Top;
	int lastRow = StringGrid->Selection.Bottom;

	if (firstRow >= 0 && lastRow < StringGrid->RowCount && firstRow <= lastRow) {

		String rowsToDelete;
        for (int i = firstRow; i <= lastRow; i++) {
			rowsToDelete += "Группу " + IntToStr(i) + "\n";
        }

		if (MessageDlg("Вы уверены, что хотите удалить следующие строки?\n" + rowsToDelete, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes) {
			for (int i = firstRow; i <= lastRow; i++) {
				for (int j = 0; j < StringGrid->ColCount; j++) {
					StringGrid->Cells[j][i] = StringGrid->Cells[j][i + (lastRow - firstRow + 1)];
                }
            }
			StringGrid->RowCount -= (lastRow - firstRow + 1);
			ShowMessage("Группа удалёна!");
        }
	}
	UpdateComboBoxes();
	UpdateHeaders();
}

// Добавление нового экзамена
void TForm1::AddExam()
{
	int newCol = StringGrid->ColCount;
	StringGrid->ColCount++;
	StringGrid->Cells[newCol][0] = "Экзамен " + IntToStr(newCol);
	for (int row = 1; row < StringGrid->RowCount; row++) {
		StringGrid->Cells[newCol][row] = "";
    }
	ShowMessage("Добавлен новый экзамен!");
	UpdateComboBoxes();
}

// Удаление экзамена
void TForm1::DelExam()
{
	int firstCol = StringGrid->Selection.Left;
	int lastCol = StringGrid->Selection.Right;

	if (firstCol >= 0 && lastCol < StringGrid->ColCount && firstCol <= lastCol) {

        String colsToDelete;
        for (int i = firstCol; i <= lastCol; i++) {
			colsToDelete += "Экзамен " + IntToStr(i) + "\n";
		}

		if (MessageDlg("Вы уверены, что хотите удалить следующие столбцы?\n" + colsToDelete, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes) {
			for (int i = 0; i < StringGrid->RowCount; i++) {
				for (int j = firstCol; j < StringGrid->ColCount - 1; j++) {
					StringGrid->Cells[j][i] = StringGrid->Cells[j + 1][i];
                }
            }
			StringGrid->ColCount -= (lastCol - firstCol + 1);
			ShowMessage("Экзамен удалён!");
		}
	}
	UpdateComboBoxes();
	UpdateHeaders();
}


// Парсинг оценок
std::vector<int> TForm1::ParseGrades(const UnicodeString &grades) {
	std::vector<int> counts(4, 0);
    UnicodeString sanitized = grades.Trim();
	sanitized = StringReplace(sanitized, L",", L" ", TReplaceFlags() << rfReplaceAll);

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string utf8_string = converter.to_bytes(sanitized.c_str());

    std::istringstream stream(utf8_string);
    int value;
    int index = 0;

    while (stream >> value) {
        if (index >= 4) {
			ShowMessage("Введено слишком много чисел! Ожидается ровно четыре оценки!");
            return {};
        }
        if (value < 0) {
			ShowMessage("Оценки не могут быть отрицательными!");
            return {};
        }
        counts[index++] = value;
    }

    if (index != 4) {
		ShowMessage("Ожидается ровно четыре числа! Проверьте формат ввода!");
        return {};
    }

    return counts;
}

// Сохранение данных в двоичный файл
void TForm1::SaveToBinaryFile() {

	try {
        std::ofstream file("grades.bin", std::ios::binary | std::ios::out);
        if (!file.is_open()) throw std::ios::failure("Не удалось открыть файл для записи.");

		const int totalGroups = StringGrid->RowCount - 1;
		const int totalExams = StringGrid->ColCount - 1;

		file.write(reinterpret_cast<const char*>(&totalGroups), sizeof(totalGroups));
		file.write(reinterpret_cast<const char*>(&totalExams), sizeof(totalExams));

		for (int row = 1; row <= totalGroups; row++) {
			for (int col = 1; col <= totalExams; col++) {
				auto cellContent = StringGrid->Cells[col][row];
				if (cellContent.IsEmpty()) {
					ShowMessage("Ячейка (" + IntToStr(row) + ", " + IntToStr(col) + ") пуста. Пропускаем её.");
					continue;
				}

				std::vector<int> counts = ParseGrades(cellContent);
				if (counts.size() != 4) {
					throw std::runtime_error("Некорректные данные в ячейке (" +
											  std::to_string(row) + ", " +
											  std::to_string(col) + ")");
				}

				file.write(reinterpret_cast<const char*>(counts.data()), counts.size() * sizeof(int));
            }
		}

		file.close();
		ShowMessage("Данные успешно сохранены в grades.bin.");
	} catch (const std::exception &e) {
        ShowMessage("Ошибка при сохранении данных: " + UnicodeString(e.what()));
	}
}

void TForm1::SaveAsToBinaryFile()
{
	if (FileSaveDialog->Execute())
	{
		UnicodeString fileName = FileSaveDialog->FileName;

		try {
			std::ofstream file(fileName.c_str(), std::ios::binary | std::ios::out);
			if (!file.is_open()) throw std::ios::failure("Не удалось открыть файл для записи.");

			const int totalGroups = StringGrid->RowCount - 1;
			const int totalExams = StringGrid->ColCount - 1;

			file.write(reinterpret_cast<const char*>(&totalGroups), sizeof(totalGroups));
			file.write(reinterpret_cast<const char*>(&totalExams), sizeof(totalExams));

			for (int row = 1; row <= totalGroups; row++) {
				for (int col = 1; col <= totalExams; col++) {
					auto cellContent = StringGrid->Cells[col][row];
					if (cellContent.IsEmpty()) {
						ShowMessage("Ячейка (" + IntToStr(row) + ", " + IntToStr(col) + ") пуста. Пропускаем её.");
						continue;
					}

					std::vector<int> counts = ParseGrades(cellContent);
					if (counts.size() != 4) {
						throw std::runtime_error("Некорректные данные в ячейке (" +
												std::to_string(row) + ", " +
												std::to_string(col) + ")");
					}

					file.write(reinterpret_cast<const char*>(counts.data()), counts.size() * sizeof(int));
				}
			}

			file.close();
			ShowMessage("Данные успешно сохранены!");
		} catch (const std::exception &e) {
			ShowMessage("Ошибка при сохранении данных: " + UnicodeString(e.what()));
		}
	}

}

// Загрузка данных из двоичного файла
void TForm1::LoadFromBinaryFile()
{
	if (FileOpenDialog->Execute())
	{
		UnicodeString fileName = FileOpenDialog->FileName;
		try {
			std::ifstream file(fileName.c_str(), std::ios::binary | std::ios::in);
			if (!file.is_open()) throw std::ios::failure("Не удалось открыть файл для чтения!");

            int totalGroups = 0, totalExams = 0;
			file.read(reinterpret_cast<char*>(&totalGroups), sizeof(totalGroups));
			file.read(reinterpret_cast<char*>(&totalExams), sizeof(totalExams));

			if (totalGroups + 1 > StringGrid->RowCount || totalExams + 1 > StringGrid->ColCount) {
				throw std::runtime_error("Размеры файла не соответствуют размеру таблицы!");
			}

			for (int row = 1; row <= totalGroups; row++) {
				for (int col = 1; col <= totalExams; col++) {
					std::vector<int> counts(4, 0);
					file.read(reinterpret_cast<char*>(counts.data()), counts.size() * sizeof(int));
					StringGrid->Cells[col][row] = IntToStr(counts[0]) + L"," +
												IntToStr(counts[1]) + L"," +
												IntToStr(counts[2]) + L"," +
												IntToStr(counts[3]);
				}
			}

			file.close();
			ShowMessage("Данные успешно загружены!");
			UpdateComboBoxes();
		} catch (const std::exception &e) {
			ShowMessage("Ошибка при загрузке данных: " + UnicodeString(e.what()));
		}
	}
}

// Очищение таблицы
void TForm1::ClearTable()
{
	for (int row = 1; row < StringGrid->RowCount; row++) {
		for (int col = 1; col < StringGrid->ColCount; col++) {
			StringGrid->Cells[col][row] = "";
        }
    }
	Memo->Lines->Clear();
	ShowMessage("Таблица очищена!");
}

// Отображение статистики
void TForm1::ShowStatistics()
{
	Memo->Lines->Clear();

	const int totalGroups = StringGrid->RowCount - 1;
	const int totalExams = StringGrid->ColCount - 1;

    std::vector<int> totalExamCounts(totalExams * 4, 0);
    std::vector<double> totalExamScores(totalExams, 0.0);
    std::vector<int> totalExamEntries(totalExams, 0);

    double flowTotalScores = 0.0;
    int flowTotalCount = 0;

    for (int row = 1; row <= totalGroups; row++) {
		Memo->Lines->Add(StringGrid->Cells[0][row] + ":");
        double groupTotalScores = 0.0;
        int groupTotalCount = 0;

        for (int col = 1; col <= totalExams; col++) {
			auto cellContent = StringGrid->Cells[col][row];
            if (cellContent.IsEmpty()) {
				Memo->Lines->Add("  Экзамен " + IntToStr(col) + ": ячейка пуста.");
                continue;
            }

            auto counts = ParseGrades(cellContent);
            if (counts.empty()) {
				Memo->Lines->Add("  Экзамен " + IntToStr(col) + ": введены некорректные данные.");
                continue;
            }

            int totalForExam = std::accumulate(counts.begin(), counts.end(), 0);
            double totalScore = counts[0] * 2 + counts[1] * 3 + counts[2] * 4 + counts[3] * 5;

            groupTotalScores += totalScore;
            groupTotalCount += totalForExam;

            totalExamScores[col - 1] += totalScore;
            totalExamEntries[col - 1] += totalForExam;

            for (int i = 0; i < 4; i++) {
                totalExamCounts[(col - 1) * 4 + i] += counts[i];
            }

            double examAvg = (totalForExam > 0) ? totalScore / totalForExam : 0.0;
			Memo->Lines->Add("  Экзамен " + IntToStr(col) + ":");
			Memo->Lines->Add("    Средний балл = " + FormatFloat("0.00", examAvg));

			Memo->Lines->Add("    Количество оценок: '2' = " + IntToStr(counts[0]) +
                              ", '3' = " + IntToStr(counts[1]) +
                              ", '4' = " + IntToStr(counts[2]) +
                              ", '5' = " + IntToStr(counts[3]));

            if (totalForExam > 0) {
				Memo->Lines->Add("    Проценты оценок: '2' = " +
                                  FormatFloat("0.00", (double(counts[0]) / totalForExam) * 100) +
                                  "%, '3' = " +
                                  FormatFloat("0.00", (double(counts[1]) / totalForExam) * 100) +
                                  "%, '4' = " +
                                  FormatFloat("0.00", (double(counts[2]) / totalForExam) * 100) +
                                  "%, '5' = " +
                                  FormatFloat("0.00", (double(counts[3]) / totalForExam) * 100) + "%");
            }
        }

        double groupAvgAllExams = (groupTotalCount > 0) ? groupTotalScores / groupTotalCount : 0.0;
		Memo->Lines->Add("Средний балл по всем экзаменам в группе: " + FormatFloat("0.00", groupAvgAllExams));

        flowTotalScores += groupTotalScores;
        flowTotalCount += groupTotalCount;
    }

	Memo->Lines->Add("Общая статистика по потоку:");

    for (int col = 1; col <= totalExams; col++) {
        int offset = (col - 1) * 4;
        double examAvg = (totalExamEntries[col - 1] > 0) ? totalExamScores[col - 1] / totalExamEntries[col - 1] : 0.0;

		Memo->Lines->Add("Экзамен " + IntToStr(col) + ":");
		Memo->Lines->Add("    Средний балл = " + FormatFloat("0.00", examAvg));
		Memo->Lines->Add("    Количество оценок: '2' = " + IntToStr(totalExamCounts[offset + 0]) +
                          ", '3' = " + IntToStr(totalExamCounts[offset + 1]) +
                          ", '4' = " + IntToStr(totalExamCounts[offset + 2]) +
                          ", '5' = " + IntToStr(totalExamCounts[offset + 3]));

        int totalForExam = totalExamEntries[col - 1];
		if (totalForExam > 0) {
			Memo->Lines->Add("    Проценты оценок: '2' = " +
                              FormatFloat("0.00", (double(totalExamCounts[offset + 0]) / totalForExam) * 100) +
                              "%, '3' = " +
                              FormatFloat("0.00", (double(totalExamCounts[offset + 1]) / totalForExam) * 100) +
							  "%, '4' = " +
                              FormatFloat("0.00", (double(totalExamCounts[offset + 2]) / totalForExam) * 100) +
                              "%, '5' = " +
                              FormatFloat("0.00", (double(totalExamCounts[offset + 3]) / totalForExam) * 100) + "%");
        }
    }

    double flowAvgAllExams = (flowTotalCount > 0) ? flowTotalScores / flowTotalCount : 0.0;
	Memo->Lines->Add("Средний балл по всем экзаменам на потоке: " + FormatFloat("0.00", flowAvgAllExams));
}

// Сохранение текстового результата
void  __fastcall TForm1::ButtonSaveTxtClick(TObject *Sender)
{
	if (SaveDialogTxt->Execute()) {
		try {
			Memo->Lines->SaveToFile(SaveDialogTxt->FileName);
			ShowMessage("Файл успешно сохранён в: " + SaveDialogTxt->FileName);

		} catch (const Exception &e) {
            ShowMessage("Ошибка при сохранении файла: " + e.Message);
        }
	}
	delete SaveDialogTxt;
}

// Сохранение графичекого результата
void  __fastcall TForm1::ButtonSaveChartClick(TObject *Sender)
{
	if (SaveDialogChart->Execute()) {
		try {

            TBitmap *Bitmap = new TBitmap();
			try {

                Bitmap->Width = Chart->Width;
                Bitmap->Height = Chart->Height;

                Chart->PaintTo(Bitmap->Canvas, 0, 0);

				Bitmap->SaveToFile(SaveDialogChart->FileName);
                ShowMessage("Файл успешно сохранён в: " + SaveDialogChart->FileName);
            }
            __finally {
                delete Bitmap;
            }
		}
        catch (const Exception &e) {
            ShowMessage("Ошибка при сохранении файла: " + e.Message);
        }
	}
	delete SaveDialogChart;
}

//Блокировка введения неккоректных символов
void __fastcall TForm1::StringGridKeyPress(TObject *Sender, System::WideChar &Key)
{
    int currentRow = StringGrid->Row;
	int currentCol = StringGrid->Col;
	String currentText = StringGrid->Cells[currentCol][currentRow];

   if (!(Key >= '0' && Key <= '9') && Key != VK_BACK && Key != ',') {
		Key = 0;
	}

	if (Key == ',' && currentText.Length() > 0) {
		if (currentText[currentText.Length()] == ',') {
			Key = 0;
        }
    }
}
