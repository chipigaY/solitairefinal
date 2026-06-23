#pragma once
#include "Records.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

public ref class RecordsForm : public Form
{
private:
    Button^ btnClose;
    Label^ lblTitle;
    Label^ lblKlondikeTitle;
    Label^ lblSpiderTitle;
    ListBox^ lstKlondike;
    ListBox^ lstSpider;
    Panel^ panelMain;

public:
    RecordsForm()
    {
        InitializeComponent();
        LoadRecords();
    }

private:
    void InitializeComponent()
    {
        this->Text = L"🏆 Рекорды";
        this->Size = Drawing::Size(650, 420);
        this->StartPosition = FormStartPosition::CenterScreen;
        this->BackColor = Color::FromArgb(0, 70, 0);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        // Заголовок
        lblTitle = gcnew Label();
        lblTitle->Text = L"🏆 Рекорды";
        lblTitle->Location = Point(225, 15);
        lblTitle->Size = Drawing::Size(200, 40);
        lblTitle->Font = gcnew System::Drawing::Font(L"Times New Roman", 24.0f, FontStyle::Bold);
        lblTitle->ForeColor = Color::Gold;
        lblTitle->TextAlign = ContentAlignment::MiddleCenter;
        this->Controls->Add(lblTitle);

        // Заголовок Косынка
        lblKlondikeTitle = gcnew Label();
        lblKlondikeTitle->Text = L"♣ Косынка";
        lblKlondikeTitle->Location = Point(40, 70);
        lblKlondikeTitle->Size = Drawing::Size(260, 25);
        lblKlondikeTitle->Font = gcnew System::Drawing::Font(L"Times New Roman", 14.0f, FontStyle::Bold);
        lblKlondikeTitle->ForeColor = Color::LightGreen;
        lblKlondikeTitle->TextAlign = ContentAlignment::MiddleCenter;
        this->Controls->Add(lblKlondikeTitle);

        // Заголовок Паук
        lblSpiderTitle = gcnew Label();
        lblSpiderTitle->Text = L"🕷 Паук";
        lblSpiderTitle->Location = Point(350, 70);
        lblSpiderTitle->Size = Drawing::Size(260, 25);
        lblSpiderTitle->Font = gcnew System::Drawing::Font(L"Times New Roman", 14.0f, FontStyle::Bold);
        lblSpiderTitle->ForeColor = Color::LightGreen;
        lblSpiderTitle->TextAlign = ContentAlignment::MiddleCenter;
        this->Controls->Add(lblSpiderTitle);

        // Список рекордов Косынка
        lstKlondike = gcnew ListBox();
        lstKlondike->Location = Point(40, 100);
        lstKlondike->Size = Drawing::Size(260, 220);
        lstKlondike->Font = gcnew System::Drawing::Font(L"Consolas", 11.0f);
        lstKlondike->BackColor = Color::FromArgb(0, 50, 0);
        lstKlondike->ForeColor = Color::FromArgb(200, 255, 200);
        lstKlondike->BorderStyle = BorderStyle::Fixed3D;
        lstKlondike->HorizontalScrollbar = true;  // Добавляем горизонтальную прокрутку
        this->Controls->Add(lstKlondike);

        // Список рекордов Паук
        lstSpider = gcnew ListBox();
        lstSpider->Location = Point(350, 100);
        lstSpider->Size = Drawing::Size(260, 220);
        lstSpider->Font = gcnew System::Drawing::Font(L"Consolas", 11.0f);
        lstSpider->BackColor = Color::FromArgb(0, 50, 0);
        lstSpider->ForeColor = Color::FromArgb(200, 255, 200);
        lstSpider->BorderStyle = BorderStyle::Fixed3D;
        lstSpider->HorizontalScrollbar = true;  // Добавляем горизонтальную прокрутку
        this->Controls->Add(lstSpider);

        // Кнопка закрыть
        btnClose = gcnew Button();
        btnClose->Text = L"✕ Закрыть";
        btnClose->Location = Point(250, 340);
        btnClose->Size = Drawing::Size(150, 35);
        btnClose->Font = gcnew System::Drawing::Font(L"Times New Roman", 12.0f, FontStyle::Bold);
        btnClose->BackColor = Color::FromArgb(40, 15, 15);
        btnClose->ForeColor = Color::FromArgb(255, 200, 200);
        btnClose->FlatStyle = FlatStyle::Standard;
        btnClose->Cursor = Cursors::Hand;
        btnClose->Click += gcnew EventHandler(this, &RecordsForm::btnClose_Click);
        btnClose->MouseEnter += gcnew EventHandler(this, &RecordsForm::btnClose_MouseEnter);
        btnClose->MouseLeave += gcnew EventHandler(this, &RecordsForm::btnClose_MouseLeave);
        this->Controls->Add(btnClose);

        // Декоративная линия
        Label^ line = gcnew Label();
        line->Location = Point(30, 60);
        line->Size = Drawing::Size(590, 2);
        line->BackColor = Color::FromArgb(0, 100, 0);
        this->Controls->Add(line);
    }

    void LoadRecords()
    {
        lstKlondike->Items->Clear();
        lstSpider->Items->Clear();

        // Загружаем рекорды Косынка
        List<RecordEntry^>^ klondikeRecords = Records::GetKlondikeRecords();
        if (klondikeRecords->Count == 0)
        {
            lstKlondike->Items->Add(L"     Нет рекордов");
        }
        else
        {
            // Добавляем заголовок таблицы
            lstKlondike->Items->Add(L"  №  Игрок             Ходов");
            lstKlondike->Items->Add(L"  ──────────────────────────");

            for (int i = 0; i < klondikeRecords->Count; i++)
            {
                RecordEntry^ entry = klondikeRecords[i];
                String^ name = entry->PlayerName;
                if (name->Length > 15) name = name->Substring(0, 15);

                String^ text = String::Format(L"  {0,2}.  {1,-15}  {2,4}",
                    i + 1,
                    name,
                    entry->Moves);
                lstKlondike->Items->Add(text);
            }
        }

        // Загружаем рекорды Паук
        List<RecordEntry^>^ spiderRecords = Records::GetSpiderRecords();
        if (spiderRecords->Count == 0)
        {
            lstSpider->Items->Add(L"     Нет рекордов");
        }
        else
        {
            // Добавляем заголовок таблицы
            lstSpider->Items->Add(L"  №  Игрок             Ходов");
            lstSpider->Items->Add(L"  ──────────────────────────");

            for (int i = 0; i < spiderRecords->Count; i++)
            {
                RecordEntry^ entry = spiderRecords[i];
                String^ name = entry->PlayerName;
                if (name->Length > 15) name = name->Substring(0, 15);

                String^ text = String::Format(L"  {0,2}.  {1,-15}  {2,4}",
                    i + 1,
                    name,
                    entry->Moves);
                lstSpider->Items->Add(text);
            }
        }
    }

    void btnClose_Click(Object^ sender, EventArgs^ e)
    {
        this->Close();
    }

    void btnClose_MouseEnter(Object^ sender, EventArgs^ e)
    {
        btnClose->BackColor = Color::FromArgb(70, 25, 25);
        btnClose->ForeColor = Color::Gold;
    }

    void btnClose_MouseLeave(Object^ sender, EventArgs^ e)
    {
        btnClose->BackColor = Color::FromArgb(40, 15, 15);
        btnClose->ForeColor = Color::FromArgb(255, 200, 200);
    }
};