#pragma once
#include "GameForm.h"
#include "SpiderGameForm.h"
#include "RecordsForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

public ref class MainMenuForm : public Form
{
private:
    Label^ lblTitle;
    Button^ btnKlondike;
    Button^ btnSpider;
    Button^ btnRecords;
    Button^ btnExit;
    Panel^ panelTitle;
    Label^ lblSubtitle;

public:
    MainMenuForm()
    {
        InitializeComponent();
    }

private:
    void InitializeComponent()
    {
        this->Text = L"Пасьянсы";
        this->Size = Drawing::Size(500, 490);
        this->StartPosition = FormStartPosition::CenterScreen;
        this->BackColor = Color::FromArgb(0, 70, 0);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        panelTitle = gcnew Panel();
        panelTitle->Location = Point(0, 0);
        panelTitle->Size = Drawing::Size(500, 110);
        panelTitle->BackColor = Color::FromArgb(0, 50, 0);
        panelTitle->BorderStyle = BorderStyle::Fixed3D;
        this->Controls->Add(panelTitle);

        lblTitle = gcnew Label();
        lblTitle->Text = L"♠ ♥ ♣ ♦";
        lblTitle->Location = Point(150, 15);
        lblTitle->Size = Drawing::Size(200, 40);
        lblTitle->Font = gcnew System::Drawing::Font(L"Times New Roman", 28.0f, FontStyle::Bold);
        lblTitle->ForeColor = Color::Gold;
        lblTitle->TextAlign = ContentAlignment::MiddleCenter;
        panelTitle->Controls->Add(lblTitle);

        lblSubtitle = gcnew Label();
        lblSubtitle->Text = L"Выберите игру";
        lblSubtitle->Location = Point(150, 60);
        lblSubtitle->Size = Drawing::Size(200, 30);
        lblSubtitle->Font = gcnew System::Drawing::Font(L"Times New Roman", 16.0f, FontStyle::Bold);
        lblSubtitle->ForeColor = Color::LightGreen;
        lblSubtitle->TextAlign = ContentAlignment::MiddleCenter;
        panelTitle->Controls->Add(lblSubtitle);


        btnKlondike = gcnew Button();
        btnKlondike->Text = L"♣ Косынка";
        btnKlondike->Location = Point(75, 140);
        btnKlondike->Size = Drawing::Size(350, 45);
        btnKlondike->Font = gcnew System::Drawing::Font(L"Times New Roman", 16.0f, FontStyle::Bold);
        btnKlondike->BackColor = Color::FromArgb(0, 60, 0);
        btnKlondike->ForeColor = Color::FromArgb(200, 255, 200);
        btnKlondike->FlatStyle = FlatStyle::Standard;
        btnKlondike->Cursor = Cursors::Hand;
        btnKlondike->Click += gcnew EventHandler(this, &MainMenuForm::btnKlondike_Click);
        btnKlondike->MouseEnter += gcnew EventHandler(this, &MainMenuForm::btn_MouseEnter);
        btnKlondike->MouseLeave += gcnew EventHandler(this, &MainMenuForm::btn_MouseLeave);
        this->Controls->Add(btnKlondike);

        btnSpider = gcnew Button();
        btnSpider->Text = L"🕷 Паук";
        btnSpider->Location = Point(75, 195);
        btnSpider->Size = Drawing::Size(350, 45);
        btnSpider->Font = gcnew System::Drawing::Font(L"Times New Roman", 16.0f, FontStyle::Bold);
        btnSpider->BackColor = Color::FromArgb(0, 60, 0);
        btnSpider->ForeColor = Color::FromArgb(200, 255, 200);
        btnSpider->FlatStyle = FlatStyle::Standard;
        btnSpider->Cursor = Cursors::Hand;
        btnSpider->Click += gcnew EventHandler(this, &MainMenuForm::btnSpider_Click);
        btnSpider->MouseEnter += gcnew EventHandler(this, &MainMenuForm::btn_MouseEnter);
        btnSpider->MouseLeave += gcnew EventHandler(this, &MainMenuForm::btn_MouseLeave);
        this->Controls->Add(btnSpider);

        btnRecords = gcnew Button();
        btnRecords->Text = L"🏆 Рекорды";
        btnRecords->Location = Point(75, 255);
        btnRecords->Size = Drawing::Size(350, 45);
        btnRecords->Font = gcnew System::Drawing::Font(L"Times New Roman", 16.0f, FontStyle::Bold);
        btnRecords->BackColor = Color::FromArgb(0, 60, 0);
        btnRecords->ForeColor = Color::FromArgb(200, 255, 200);
        btnRecords->FlatStyle = FlatStyle::Standard;
        btnRecords->Cursor = Cursors::Hand;
        btnRecords->Click += gcnew EventHandler(this, &MainMenuForm::btnRecords_Click);
        btnRecords->MouseEnter += gcnew EventHandler(this, &MainMenuForm::btn_MouseEnter);
        btnRecords->MouseLeave += gcnew EventHandler(this, &MainMenuForm::btn_MouseLeave);
        this->Controls->Add(btnRecords);

        btnExit = gcnew Button();
        btnExit->Text = L"✕ Выход";
        btnExit->Location = Point(175, 320);
        btnExit->Size = Drawing::Size(150, 40);
        btnExit->Font = gcnew System::Drawing::Font(L"Times New Roman", 12.0f, FontStyle::Bold);
        btnExit->BackColor = Color::FromArgb(40, 15, 15);
        btnExit->ForeColor = Color::FromArgb(255, 200, 200);
        btnExit->FlatStyle = FlatStyle::Standard;
        btnExit->Cursor = Cursors::Hand;
        btnExit->Click += gcnew EventHandler(this, &MainMenuForm::btnExit_Click);
        btnExit->MouseEnter += gcnew EventHandler(this, &MainMenuForm::btnExit_MouseEnter);
        btnExit->MouseLeave += gcnew EventHandler(this, &MainMenuForm::btnExit_MouseLeave);
        this->Controls->Add(btnExit);

        // Разделительные линии
        Label^ line1 = gcnew Label();
        line1->Location = Point(50, 125);
        line1->Size = Drawing::Size(400, 2);
        line1->BackColor = Color::FromArgb(0, 100, 0);
        this->Controls->Add(line1);

        Label^ line2 = gcnew Label();
        line2->Location = Point(50, 310);
        line2->Size = Drawing::Size(400, 2);
        line2->BackColor = Color::FromArgb(0, 100, 0);
        this->Controls->Add(line2);

        // Декоративная рамка
        Panel^ borderPanel = gcnew Panel();
        borderPanel->Location = Point(60, 132);
        borderPanel->Size = Drawing::Size(380, 175);
        borderPanel->BackColor = Color::Transparent;
        borderPanel->BorderStyle = BorderStyle::FixedSingle;
        borderPanel->Enabled = false;
        this->Controls->Add(borderPanel);
    }

    void btn_MouseEnter(Object^ sender, EventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        btn->BackColor = Color::FromArgb(0, 90, 0);
        btn->ForeColor = Color::Gold;
    }

    void btn_MouseLeave(Object^ sender, EventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        btn->BackColor = Color::FromArgb(0, 60, 0);
        btn->ForeColor = Color::FromArgb(200, 255, 200);
    }

    void btnExit_MouseEnter(Object^ sender, EventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        btn->BackColor = Color::FromArgb(70, 25, 25);
        btn->ForeColor = Color::Gold;
    }

    void btnExit_MouseLeave(Object^ sender, EventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        btn->BackColor = Color::FromArgb(40, 15, 15);
        btn->ForeColor = Color::FromArgb(255, 200, 200);
    }

    void btnRecords_Click(Object^ sender, EventArgs^ e)
    {
        RecordsForm^ form = gcnew RecordsForm();
        form->ShowDialog();
    }

    void btnKlondike_Click(Object^ sender, EventArgs^ e)
    {
        this->Hide();
        GameForm^ form = gcnew GameForm();
        form->ShowDialog();
        this->Show();
    }

    void btnSpider_Click(Object^ sender, EventArgs^ e)
    {
        this->Hide();
        SpiderGameForm^ form = gcnew SpiderGameForm(1);
        form->ShowDialog();
        this->Show();
    }

    void btnExit_Click(Object^ sender, EventArgs^ e)
    {
        Application::Exit();
    }
};
