#pragma once
#include "Card.h"
#include "Deck.h"
#include "Pile.h"
#include "Records.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

public ref class GameForm : public Form
{
private:
    Deck^ deck;
    List<Pile^>^ tableauPiles;
    List<Pile^>^ foundationPiles;
    Pile^ stockPile;
    Pile^ wastePile;

    // Drag & Drop переменные
    bool isDragging;
    Pile^ dragSourcePile;
    int dragStartIndex;
    Point dragStartPoint;
    List<Card^>^ dragCards;

    // Масштабирование
    float scaleFactor;
    int cardWidth;
    int cardHeight;
    int pileOffset;
    int startX;
    int startY;
    int topY;

    // Счетчики игры
    int movesCount;

    Button^ btnNewGame;
    Button^ btnDeal;
    Button^ btnBack;
    Label^ lblStatus;
    Label^ lblMoves;
    Label^ lblScore;

public:
    GameForm()
    {
        movesCount = 0;
        scaleFactor = 1.0f;
        cardWidth = 70;
        cardHeight = 100;
        pileOffset = 25;
        startX = 50;
        startY = 200;
        topY = 80;
        InitializeComponent();
        InitializeGame();
    }

private:
    void InitializeComponent()
    {
        this->Text = L"Пасьянс Косынка";
        this->Size = Drawing::Size(900, 700);
        this->StartPosition = FormStartPosition::CenterScreen;
        this->BackColor = Color::DarkGreen;
        this->DoubleBuffered = true;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
        this->MinimumSize = Drawing::Size(800, 600);
        this->MaximizeBox = true;
        this->MinimizeBox = true;

        // КНОПКА "← Меню"
        btnBack = gcnew Button();
        btnBack->Text = L"← Меню";
        btnBack->Location = Point(10, 10);
        btnBack->Size = Drawing::Size(80, 30);
        btnBack->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        btnBack->BackColor = Color::FromArgb(60, 30, 30);
        btnBack->ForeColor = Color::LightSalmon;
        btnBack->FlatStyle = FlatStyle::Flat;
        btnBack->FlatAppearance->BorderSize = 2;
        btnBack->FlatAppearance->BorderColor = Color::DarkRed;
        btnBack->Cursor = Cursors::Hand;
        btnBack->Click += gcnew EventHandler(this, &GameForm::btnBack_Click);
        btnBack->MouseEnter += gcnew EventHandler(this, &GameForm::btn_MouseEnter);
        btnBack->MouseLeave += gcnew EventHandler(this, &GameForm::btn_MouseLeave);
        this->Controls->Add(btnBack);

        // КНОПКА "Новая игра"
        btnNewGame = gcnew Button();
        btnNewGame->Text = L"Новая игра";
        btnNewGame->Location = Point(100, 10);
        btnNewGame->Size = Drawing::Size(100, 30);
        btnNewGame->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        btnNewGame->BackColor = Color::FromArgb(0, 80, 0);
        btnNewGame->ForeColor = Color::Gold;
        btnNewGame->FlatStyle = FlatStyle::Flat;
        btnNewGame->FlatAppearance->BorderSize = 2;
        btnNewGame->FlatAppearance->BorderColor = Color::Gold;
        btnNewGame->Cursor = Cursors::Hand;
        btnNewGame->Click += gcnew EventHandler(this, &GameForm::btnNewGame_Click);
        btnNewGame->MouseEnter += gcnew EventHandler(this, &GameForm::btn_MouseEnter);
        btnNewGame->MouseLeave += gcnew EventHandler(this, &GameForm::btn_MouseLeave);
        this->Controls->Add(btnNewGame);

        // КНОПКА "Сдать карту"
        btnDeal = gcnew Button();
        btnDeal->Text = L"Сдать карту";
        btnDeal->Location = Point(210, 10);
        btnDeal->Size = Drawing::Size(100, 30);
        btnDeal->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        btnDeal->BackColor = Color::FromArgb(0, 60, 80);
        btnDeal->ForeColor = Color::LightCyan;
        btnDeal->FlatStyle = FlatStyle::Flat;
        btnDeal->FlatAppearance->BorderSize = 2;
        btnDeal->FlatAppearance->BorderColor = Color::DeepSkyBlue;
        btnDeal->Cursor = Cursors::Hand;
        btnDeal->Click += gcnew EventHandler(this, &GameForm::btnDeal_Click);
        btnDeal->MouseEnter += gcnew EventHandler(this, &GameForm::btn_MouseEnter);
        btnDeal->MouseLeave += gcnew EventHandler(this, &GameForm::btn_MouseLeave);
        this->Controls->Add(btnDeal);

        // СТАТУС (жирный)
        lblStatus = gcnew Label();
        lblStatus->Text = L"Добро пожаловать!";
        lblStatus->Location = Point(330, 15);
        lblStatus->Size = Drawing::Size(250, 20);
        lblStatus->ForeColor = Color::White;
        lblStatus->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        this->Controls->Add(lblStatus);

        // ХОДЫ (жирный)
        lblMoves = gcnew Label();
        lblMoves->Text = L"Ходов: 0";
        lblMoves->Location = Point(630, 15);
        lblMoves->Size = Drawing::Size(100, 20);
        lblMoves->ForeColor = Color::White;
        lblMoves->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        this->Controls->Add(lblMoves);

        // СОБРАНО (жирный)
        lblScore = gcnew Label();
        lblScore->Text = L"Собрано: 0/52";
        lblScore->Location = Point(740, 15);
        lblScore->Size = Drawing::Size(120, 20);
        lblScore->ForeColor = Color::White;
        lblScore->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        this->Controls->Add(lblScore);

        // Подключаем события мыши для Drag & Drop
        this->MouseDown += gcnew MouseEventHandler(this, &GameForm::GameForm_MouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &GameForm::GameForm_MouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &GameForm::GameForm_MouseUp);
        this->Paint += gcnew PaintEventHandler(this, &GameForm::GameForm_Paint);
        this->Resize += gcnew EventHandler(this, &GameForm::GameForm_Resize);

        this->SetStyle(ControlStyles::UserPaint | ControlStyles::AllPaintingInWmPaint | ControlStyles::DoubleBuffer, true);
    }

    void GameForm_Resize(Object^ sender, EventArgs^ e)
    {
        CalculateScale();
        UpdatePilePositions();
        this->Invalidate();
    }

    void CalculateScale()
    {
        float baseWidth = 900.0f;
        float baseHeight = 700.0f;

        float widthScale = (float)this->ClientSize.Width / baseWidth;
        float heightScale = (float)this->ClientSize.Height / baseHeight;

        scaleFactor = Math::Min(widthScale, heightScale);

        if (scaleFactor < 0.6f) scaleFactor = 0.6f;
        if (scaleFactor > 1.8f) scaleFactor = 1.8f;

        cardWidth = (int)(70 * scaleFactor);
        cardHeight = (int)(100 * scaleFactor);
        pileOffset = (int)(25 * scaleFactor);
        startX = (int)(50 * scaleFactor);
        startY = (int)(200 * scaleFactor);
        topY = (int)(80 * scaleFactor);

        // Обновляем позиции кнопок
        btnBack->Location = Point((int)(10 * scaleFactor), (int)(10 * scaleFactor));
        btnBack->Size = Drawing::Size((int)(80 * scaleFactor), (int)(30 * scaleFactor));
        btnNewGame->Location = Point((int)(100 * scaleFactor), (int)(10 * scaleFactor));
        btnNewGame->Size = Drawing::Size((int)(100 * scaleFactor), (int)(30 * scaleFactor));
        btnDeal->Location = Point((int)(210 * scaleFactor), (int)(10 * scaleFactor));
        btnDeal->Size = Drawing::Size((int)(100 * scaleFactor), (int)(30 * scaleFactor));
        lblStatus->Location = Point((int)(330 * scaleFactor), (int)(15 * scaleFactor));
        lblStatus->Size = Drawing::Size((int)(250 * scaleFactor), (int)(20 * scaleFactor));
        lblMoves->Location = Point((int)(630 * scaleFactor), (int)(15 * scaleFactor));
        lblMoves->Size = Drawing::Size((int)(100 * scaleFactor), (int)(20 * scaleFactor));
        lblScore->Location = Point((int)(740 * scaleFactor), (int)(15 * scaleFactor));
        lblScore->Size = Drawing::Size((int)(120 * scaleFactor), (int)(20 * scaleFactor));

        // Обновляем размер шрифта (жирный)
        int fontSize = (int)(10 * scaleFactor);
        if (fontSize < 8) fontSize = 8;
        if (fontSize > 16) fontSize = 16;

        btnBack->Font = gcnew System::Drawing::Font(L"Arial", (float)fontSize, FontStyle::Bold);
        btnNewGame->Font = gcnew System::Drawing::Font(L"Arial", (float)fontSize, FontStyle::Bold);
        btnDeal->Font = gcnew System::Drawing::Font(L"Arial", (float)fontSize, FontStyle::Bold);
        lblStatus->Font = gcnew System::Drawing::Font(L"Arial", (float)fontSize, FontStyle::Bold);
        lblMoves->Font = gcnew System::Drawing::Font(L"Arial", (float)fontSize, FontStyle::Bold);
        lblScore->Font = gcnew System::Drawing::Font(L"Arial", (float)fontSize, FontStyle::Bold);
    }

    void UpdatePilePositions()
    {
        if (tableauPiles != nullptr)
        {
            for (int i = 0; i < tableauPiles->Count; i++)
            {
                if (tableauPiles[i] != nullptr)
                {
                    int xPos = startX + i * (cardWidth + (int)(20 * scaleFactor));
                    tableauPiles[i]->SetPosition(xPos, startY);
                    tableauPiles[i]->SetCardSize(cardWidth, cardHeight, pileOffset);
                }
            }
        }

        if (foundationPiles != nullptr)
        {
            for (int i = 0; i < foundationPiles->Count; i++)
            {
                if (foundationPiles[i] != nullptr)
                {
                    int xPos = startX + (i + 3) * (cardWidth + (int)(20 * scaleFactor));
                    foundationPiles[i]->SetPosition(xPos, topY);
                    foundationPiles[i]->SetCardSize(cardWidth, cardHeight, 0);
                }
            }
        }

        if (stockPile != nullptr)
        {
            stockPile->SetPosition((int)(50 * scaleFactor), topY);
            stockPile->SetCardSize(cardWidth, cardHeight, 0);
        }

        if (wastePile != nullptr)
        {
            wastePile->SetPosition((int)(50 * scaleFactor) + cardWidth + (int)(20 * scaleFactor), topY);
            wastePile->SetCardSize(cardWidth, cardHeight, 0);
        }
    }

    void btn_MouseEnter(Object^ sender, EventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        if (btn == btnBack)
        {
            btn->BackColor = Color::FromArgb(100, 40, 40);
            btn->FlatAppearance->BorderColor = Color::Tomato;
        }
        else if (btn == btnNewGame)
        {
            btn->BackColor = Color::FromArgb(0, 120, 0);
            btn->FlatAppearance->BorderColor = Color::Yellow;
        }
        else if (btn == btnDeal)
        {
            btn->BackColor = Color::FromArgb(0, 90, 120);
            btn->FlatAppearance->BorderColor = Color::DodgerBlue;
        }
    }

    void btn_MouseLeave(Object^ sender, EventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        if (btn == btnBack)
        {
            btn->BackColor = Color::FromArgb(60, 30, 30);
            btn->FlatAppearance->BorderColor = Color::DarkRed;
        }
        else if (btn == btnNewGame)
        {
            btn->BackColor = Color::FromArgb(0, 80, 0);
            btn->FlatAppearance->BorderColor = Color::Gold;
        }
        else if (btn == btnDeal)
        {
            btn->BackColor = Color::FromArgb(0, 60, 80);
            btn->FlatAppearance->BorderColor = Color::DeepSkyBlue;
        }
    }

    void UpdateStats()
    {
        lblMoves->Text = L"Ходов: " + movesCount.ToString();

        int total = 0;
        if (foundationPiles != nullptr)
        {
            for each (Pile ^ pile in foundationPiles)
            {
                if (pile != nullptr)
                    total += pile->GetSize();
            }
        }
        lblScore->Text = L"Собрано: " + total.ToString() + L"/52";
    }

    void InitializeGame()
    {
        try
        {
            movesCount = 0;
            deck = gcnew Deck();
            deck->Shuffle();

            tableauPiles = gcnew List<Pile^>();
            foundationPiles = gcnew List<Pile^>();

            CalculateScale();

            stockPile = gcnew Pile((int)(50 * scaleFactor), topY, false, false);
            wastePile = gcnew Pile((int)(50 * scaleFactor) + cardWidth + (int)(20 * scaleFactor), topY, false, false);

            for (int i = 0; i < 4; i++)
            {
                int xPos = startX + (i + 3) * (cardWidth + (int)(20 * scaleFactor));
                foundationPiles->Add(gcnew Pile(xPos, topY, true, false));
            }

            for (int i = 0; i < 7; i++)
            {
                int xPos = startX + i * (cardWidth + (int)(20 * scaleFactor));
                Pile^ pile = gcnew Pile(xPos, startY, false, true);
                pile->SetCardSize(cardWidth, cardHeight, pileOffset);
                for (int j = 0; j <= i; j++)
                {
                    Card^ card = deck->DealCard();
                    if (card != nullptr)
                    {
                        card->SetFaceUp(j == i);
                        pile->AddCard(card);
                    }
                }
                tableauPiles->Add(pile);
            }

            while (!deck->IsEmpty())
            {
                Card^ card = deck->DealCard();
                if (card != nullptr)
                {
                    card->SetFaceUp(false);
                    stockPile->AddCard(card);
                }
            }

            isDragging = false;
            dragSourcePile = nullptr;
            dragStartIndex = -1;
            dragCards = nullptr;

            UpdateStats();
            this->Invalidate();
            lblStatus->Text = L"Новая игра начата! Перетаскивайте карты мышкой";
        }
        catch (Exception^ ex)
        {
            MessageBox::Show("Ошибка: " + ex->Message);
        }
    }

    void GameForm_Paint(Object^ sender, PaintEventArgs^ e)
    {
        try
        {
            Graphics^ g = e->Graphics;

            if (stockPile != nullptr) stockPile->DrawPile(g);
            if (wastePile != nullptr) wastePile->DrawPile(g);

            if (foundationPiles != nullptr)
            {
                for each (Pile ^ pile in foundationPiles)
                {
                    if (pile != nullptr) pile->DrawPile(g);
                }
            }

            if (tableauPiles != nullptr)
            {
                for each (Pile ^ pile in tableauPiles)
                {
                    if (pile != nullptr) pile->DrawPile(g, pileOffset);
                }
            }

            if (isDragging && dragCards != nullptr && dragCards->Count > 0)
            {
                int x = dragStartPoint.X - (int)(35 * scaleFactor);
                int y = dragStartPoint.Y - (int)(50 * scaleFactor);

                for (int i = 0; i < dragCards->Count; i++)
                {
                    if (dragCards[i] != nullptr)
                    {
                        dragCards[i]->DrawCard(g, x, y + i * pileOffset, cardWidth, cardHeight);
                    }
                }
            }
        }
        catch (Exception^) {}
    }

    void GameForm_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (e->Button != System::Windows::Forms::MouseButtons::Left)
            return;

        try
        {
            if (tableauPiles != nullptr)
            {
                for (int i = 0; i < tableauPiles->Count; i++)
                {
                    Pile^ pile = tableauPiles[i];
                    if (pile == nullptr) continue;

                    int cardIndex = pile->GetCardIndexAtPosition(e->X, e->Y);
                    if (cardIndex >= 0)
                    {
                        bool allOpen = true;
                        for (int j = cardIndex; j < pile->GetCards()->Count; j++)
                        {
                            if (pile->GetCards()[j] == nullptr || !pile->GetCards()[j]->IsFaceUp())
                            {
                                allOpen = false;
                                break;
                            }
                        }

                        if (!allOpen)
                        {
                            lblStatus->Text = L"Нельзя перетаскивать закрытые карты";
                            return;
                        }

                        bool validSequence = true;
                        for (int j = cardIndex; j < pile->GetCards()->Count - 1; j++)
                        {
                            Card^ current = pile->GetCards()[j];
                            Card^ next = pile->GetCards()[j + 1];

                            if (current == nullptr || next == nullptr)
                            {
                                validSequence = false;
                                break;
                            }

                            if (current->GetRank() != next->GetRank() + 1 ||
                                current->GetCardColor() == next->GetCardColor())
                            {
                                validSequence = false;
                                break;
                            }
                        }

                        if (!validSequence)
                        {
                            lblStatus->Text = L"Карты должны идти по порядку";
                            return;
                        }

                        isDragging = true;
                        dragSourcePile = pile;
                        dragStartIndex = cardIndex;

                        dragCards = gcnew List<Card^>();
                        for (int j = cardIndex; j < pile->GetCards()->Count; j++)
                        {
                            if (pile->GetCards()[j] != nullptr)
                                dragCards->Add(pile->GetCards()[j]);
                        }

                        dragStartPoint = e->Location;

                        lblStatus->Text = L"Перетаскиваю " + dragCards->Count.ToString() + L" карт";
                        this->Invalidate();
                        return;
                    }
                }
            }

            if (wastePile != nullptr)
            {
                int cardIndex = wastePile->GetCardIndexAtPosition(e->X, e->Y);
                if (cardIndex >= 0)
                {
                    isDragging = true;
                    dragSourcePile = wastePile;
                    dragStartIndex = cardIndex;

                    dragCards = gcnew List<Card^>();
                    if (wastePile->GetCards()[cardIndex] != nullptr)
                        dragCards->Add(wastePile->GetCards()[cardIndex]);

                    dragStartPoint = e->Location;

                    lblStatus->Text = L"Перетаскиваю карту из сброса";
                    this->Invalidate();
                    return;
                }
            }
        }
        catch (Exception^ ex)
        {
            lblStatus->Text = L"Ошибка: " + ex->Message;
        }
    }

    void GameForm_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        if (isDragging && dragCards != nullptr)
        {
            dragStartPoint = e->Location;
            this->Invalidate();
        }
    }

    void GameForm_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (!isDragging || dragSourcePile == nullptr || dragCards == nullptr)
        {
            isDragging = false;
            return;
        }

        try
        {
            bool dropped = false;

            if (tableauPiles != nullptr)
            {
                for (int i = 0; i < tableauPiles->Count; i++)
                {
                    Pile^ pile = tableauPiles[i];
                    if (pile == nullptr || pile == dragSourcePile) continue;

                    if (pile->IsAtPosition(e->X, e->Y))
                    {
                        Card^ firstCard = dragCards[0];
                        if (firstCard == nullptr) break;

                        if (pile->IsEmpty() && firstCard->GetRank() == 13)
                        {
                            DropCards(dragSourcePile, pile, dragStartIndex);
                            dropped = true;
                            movesCount++;
                            UpdateStats();
                            lblStatus->Text = L"Король перемещен";
                            break;
                        }
                        else if (!pile->IsEmpty() && pile->CanAddCard(firstCard))
                        {
                            DropCards(dragSourcePile, pile, dragStartIndex);
                            dropped = true;
                            movesCount++;
                            UpdateStats();
                            lblStatus->Text = L"Карты перемещены";
                            break;
                        }
                        else
                        {
                            lblStatus->Text = L"Сюда нельзя положить эти карты";
                        }
                    }
                }
            }

            if (!dropped && foundationPiles != nullptr)
            {
                for (int i = 0; i < foundationPiles->Count; i++)
                {
                    Pile^ pile = foundationPiles[i];
                    if (pile == nullptr) continue;

                    if (pile->IsAtPosition(e->X, e->Y))
                    {
                        if (dragCards->Count > 1)
                        {
                            lblStatus->Text = L"В сборную стопку можно класть только по одной карте";
                            break;
                        }

                        Card^ card = dragCards[0];
                        if (card != nullptr && pile->CanAddCard(card))
                        {
                            dragSourcePile->RemoveCard();
                            pile->AddCard(card);

                            if (!dragSourcePile->IsEmpty())
                            {
                                Card^ top = dragSourcePile->GetTopCard();
                                if (top != nullptr) top->SetFaceUp(true);
                            }

                            dropped = true;
                            movesCount++;
                            UpdateStats();
                            CheckWin();
                            lblStatus->Text = L"Карта добавлена в сборную стопку";
                            break;
                        }
                        else
                        {
                            lblStatus->Text = L"Сюда нельзя положить эту карту";
                        }
                    }
                }
            }

            if (!dropped)
            {
                lblStatus->Text = L"Перемещение отменено";
            }

            if (!dragSourcePile->IsEmpty())
            {
                Card^ top = dragSourcePile->GetTopCard();
                if (top != nullptr) top->SetFaceUp(true);
            }
        }
        catch (Exception^ ex)
        {
            lblStatus->Text = L"Ошибка: " + ex->Message;
        }

        isDragging = false;
        dragSourcePile = nullptr;
        dragStartIndex = -1;
        dragCards = nullptr;
        this->Invalidate();
    }

    void DropCards(Pile^ source, Pile^ destination, int startIndex)
    {
        if (source == nullptr || destination == nullptr) return;
        if (source->GetCards() == nullptr || destination->GetCards() == nullptr) return;

        List<Card^>^ sourceCards = source->GetCards();

        List<Card^>^ cardsToMove = gcnew List<Card^>();
        for (int i = startIndex; i < sourceCards->Count; i++)
        {
            if (sourceCards[i] != nullptr)
                cardsToMove->Add(sourceCards[i]);
        }

        for (int i = sourceCards->Count - 1; i >= startIndex; i--)
        {
            sourceCards->RemoveAt(i);
        }

        for (int i = 0; i < cardsToMove->Count; i++)
        {
            if (cardsToMove[i] != nullptr)
                destination->AddCard(cardsToMove[i]);
        }

        if (!source->IsEmpty())
        {
            Card^ top = source->GetTopCard();
            if (top != nullptr) top->SetFaceUp(true);
        }

        UpdateStats();
        CheckWin();
        this->Invalidate();
    }

    void HandleStockClick()
    {
        if (stockPile == nullptr) return;

        if (stockPile->IsEmpty())
        {
            if (wastePile != nullptr && !wastePile->IsEmpty())
            {
                List<Card^>^ wasteCards = wastePile->GetCards();
                while (wasteCards->Count > 0)
                {
                    Card^ card = wasteCards[wasteCards->Count - 1];
                    wasteCards->RemoveAt(wasteCards->Count - 1);
                    if (card != nullptr)
                    {
                        card->SetFaceUp(false);
                        stockPile->AddCard(card);
                    }
                }
                lblStatus->Text = L"Колода восстановлена";
                this->Invalidate();
            }
            else
            {
                lblStatus->Text = L"Колода и сброс пусты";
            }
            return;
        }

        Card^ card = stockPile->RemoveCard();
        if (card != nullptr && wastePile != nullptr)
        {
            card->SetFaceUp(true);
            wastePile->AddCard(card);
            movesCount++;
            UpdateStats();
            lblStatus->Text = L"Сдана карта: " + card->GetRankString() + card->GetSuitSymbol();
            this->Invalidate();
        }
    }

    void CheckWin()
    {
        if (foundationPiles == nullptr) return;

        for each (Pile ^ pile in foundationPiles)
        {
            if (pile == nullptr || pile->GetSize() != 13)
                return;
        }

        // Запрос имени для рекорда
        String^ name = Microsoft::VisualBasic::Interaction::InputBox(
            L"Поздравляем! Вы выиграли за " + movesCount.ToString() + L" ходов!\nВведите ваше имя для рекорда:",
            L"Победа!",
            L"Игрок",
            -1, -1);

        if (!String::IsNullOrEmpty(name))
        {
            Records::AddRecord("Klondike", name, movesCount);
        }

        MessageBox::Show("Поздравляем! Вы выиграли!", "Победа!",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
        lblStatus->Text = L"Вы выиграли! 🎉";
    }

    void btnNewGame_Click(Object^ sender, EventArgs^ e)
    {
        // Добавлено подтверждение как в пауке
        if (MessageBox::Show("Начать новую игру?", "Подтверждение",
            MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes)
        {
            InitializeGame();
        }
    }

    void btnDeal_Click(Object^ sender, EventArgs^ e)
    {
        HandleStockClick();
    }

    void btnBack_Click(Object^ sender, EventArgs^ e)
    {
        this->Close();
    }
};
