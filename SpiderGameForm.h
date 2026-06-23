#pragma once
#include "Card.h"
#include "SpiderDeck.h"
#include "SpiderPile.h"
#include "Records.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

public ref class SpiderGameForm : public Form
{
private:
    SpiderDeck^ deck;
    List<SpiderPile^>^ tableauPiles;

    // Drag & Drop переменные
    bool isDragging;
    SpiderPile^ dragSourcePile;
    int dragStartIndex;
    Point dragStartPoint;
    List<Card^>^ dragCards;
    Point dragOffset;

    // Настройки игры
    int suitsCount;
    int movesCount;
    int completedSuits;
    int dealsRemaining;
    bool gameOver;

    // Масштабирование
    float scaleFactor;
    int cardWidth;
    int cardHeight;
    int pileOffset;
    int startX;
    int startY;
    int infoY;

    // UI элементы
    Button^ btnNewGame;
    Button^ btnDeal;
    Button^ btnBack;
    Label^ lblStatus;
    Label^ lblMoves;
    Label^ lblCompleted;
    Label^ lblCardsLeft;
    Label^ lblDealsLeft;

public:
    SpiderGameForm(int suits)
    {
        suitsCount = suits;
        scaleFactor = 1.0f;
        cardWidth = 70;
        cardHeight = 100;
        pileOffset = 25;
        startX = 15;
        startY = 170;
        infoY = 85;
        InitializeComponent();
        InitializeGame();
    }

private:
    void InitializeComponent()
    {
        this->Text = L"Пасьянс Паук - 1 масть";
        this->Size = Drawing::Size(950, 750);
        this->StartPosition = FormStartPosition::CenterScreen;
        this->BackColor = Color::DarkGreen;
        this->DoubleBuffered = true;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
        this->MaximizeBox = true;
        this->MinimizeBox = true;
        this->MinimumSize = Drawing::Size(800, 600);

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
        btnBack->Click += gcnew EventHandler(this, &SpiderGameForm::btnBack_Click);
        btnBack->MouseEnter += gcnew EventHandler(this, &SpiderGameForm::btn_MouseEnter);
        btnBack->MouseLeave += gcnew EventHandler(this, &SpiderGameForm::btn_MouseLeave);
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
        btnNewGame->Click += gcnew EventHandler(this, &SpiderGameForm::btnNewGame_Click);
        btnNewGame->MouseEnter += gcnew EventHandler(this, &SpiderGameForm::btn_MouseEnter);
        btnNewGame->MouseLeave += gcnew EventHandler(this, &SpiderGameForm::btn_MouseLeave);
        this->Controls->Add(btnNewGame);

        // КНОПКА "Сдать карты"
        btnDeal = gcnew Button();
        btnDeal->Text = L"Сдать карты";
        btnDeal->Location = Point(210, 10);
        btnDeal->Size = Drawing::Size(100, 30);
        btnDeal->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        btnDeal->BackColor = Color::FromArgb(0, 60, 80);
        btnDeal->ForeColor = Color::LightCyan;
        btnDeal->FlatStyle = FlatStyle::Flat;
        btnDeal->FlatAppearance->BorderSize = 2;
        btnDeal->FlatAppearance->BorderColor = Color::DeepSkyBlue;
        btnDeal->Cursor = Cursors::Hand;
        btnDeal->Click += gcnew EventHandler(this, &SpiderGameForm::btnDeal_Click);
        btnDeal->MouseEnter += gcnew EventHandler(this, &SpiderGameForm::btn_MouseEnter);
        btnDeal->MouseLeave += gcnew EventHandler(this, &SpiderGameForm::btn_MouseLeave);
        this->Controls->Add(btnDeal);

        // СТАТУС
        lblStatus = gcnew Label();
        lblStatus->Text = L"Добро пожаловать в Паук!";
        lblStatus->Location = Point(400, 15);
        lblStatus->Size = Drawing::Size(300, 20);
        lblStatus->ForeColor = Color::White;
        lblStatus->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        this->Controls->Add(lblStatus);

        // ХОДОВ (увеличен размер)
        lblMoves = gcnew Label();
        lblMoves->Text = L"Ходов: 0";
        lblMoves->Location = Point(10, 55);
        lblMoves->Size = Drawing::Size(100, 20);
        lblMoves->ForeColor = Color::White;
        lblMoves->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        this->Controls->Add(lblMoves);

        // СОБРАНО (увеличен размер)
        lblCompleted = gcnew Label();
        lblCompleted->Text = L"Собрано: 0/8";
        lblCompleted->Location = Point(120, 55);
        lblCompleted->Size = Drawing::Size(120, 20);
        lblCompleted->ForeColor = Color::White;
        lblCompleted->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        this->Controls->Add(lblCompleted);

        // В КОЛОДЕ (увеличен размер)
        lblCardsLeft = gcnew Label();
        lblCardsLeft->Text = L"В колоде: 50";
        lblCardsLeft->Location = Point(250, 55);
        lblCardsLeft->Size = Drawing::Size(120, 20);
        lblCardsLeft->ForeColor = Color::White;
        lblCardsLeft->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        this->Controls->Add(lblCardsLeft);

        // РАЗДАЧ (увеличен размер)
        lblDealsLeft = gcnew Label();
        lblDealsLeft->Text = L"Раздач: 5";
        lblDealsLeft->Location = Point(380, 55);
        lblDealsLeft->Size = Drawing::Size(120, 20);
        lblDealsLeft->ForeColor = Color::White;
        lblDealsLeft->Font = gcnew System::Drawing::Font(L"Arial", 10.0f, FontStyle::Bold);
        this->Controls->Add(lblDealsLeft);

        this->MouseDown += gcnew MouseEventHandler(this, &SpiderGameForm::GameForm_MouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &SpiderGameForm::GameForm_MouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &SpiderGameForm::GameForm_MouseUp);
        this->Paint += gcnew PaintEventHandler(this, &SpiderGameForm::GameForm_Paint);
        this->Resize += gcnew EventHandler(this, &SpiderGameForm::GameForm_Resize);

        this->SetStyle(ControlStyles::UserPaint | ControlStyles::AllPaintingInWmPaint | ControlStyles::DoubleBuffer, true);

        isDragging = false;
        dragSourcePile = nullptr;
        dragStartIndex = -1;
        dragCards = nullptr;
        dragOffset = Point(0, 0);
    }

    void GameForm_Resize(Object^ sender, EventArgs^ e)
    {
        CalculateScale();
        UpdatePilePositions();
        this->Invalidate();
    }

    void CalculateScale()
    {
        float baseWidth = 950.0f;
        float baseHeight = 750.0f;

        float widthScale = (float)this->ClientSize.Width / baseWidth;
        float heightScale = (float)this->ClientSize.Height / baseHeight;

        scaleFactor = Math::Min(widthScale, heightScale);

        if (scaleFactor < 0.6f) scaleFactor = 0.6f;
        if (scaleFactor > 1.8f) scaleFactor = 1.8f;

        cardWidth = (int)(70 * scaleFactor);
        cardHeight = (int)(100 * scaleFactor);
        pileOffset = (int)(25 * scaleFactor);
        startX = (int)(15 * scaleFactor);
        startY = (int)(170 * scaleFactor);
        infoY = (int)(85 * scaleFactor);

        // Обновляем позиции и размеры лейблов с увеличенной шириной
        lblMoves->Location = Point((int)(10 * scaleFactor), (int)(55 * scaleFactor));
        lblMoves->Size = Drawing::Size((int)(120 * scaleFactor), (int)(20 * scaleFactor));

        lblCompleted->Location = Point((int)(140 * scaleFactor), (int)(55 * scaleFactor));
        lblCompleted->Size = Drawing::Size((int)(130 * scaleFactor), (int)(20 * scaleFactor));

        lblCardsLeft->Location = Point((int)(280 * scaleFactor), (int)(55 * scaleFactor));
        lblCardsLeft->Size = Drawing::Size((int)(130 * scaleFactor), (int)(20 * scaleFactor));

        lblDealsLeft->Location = Point((int)(420 * scaleFactor), (int)(55 * scaleFactor));
        lblDealsLeft->Size = Drawing::Size((int)(120 * scaleFactor), (int)(20 * scaleFactor));

        lblStatus->Location = Point((int)(400 * scaleFactor), (int)(15 * scaleFactor));
        lblStatus->Size = Drawing::Size((int)(350 * scaleFactor), (int)(20 * scaleFactor));

        // Вычисляем размер шрифта
        int fontSize = (int)(10 * scaleFactor);
        if (fontSize < 8) fontSize = 8;
        if (fontSize > 16) fontSize = 16;

        System::Drawing::Font^ newFont = gcnew System::Drawing::Font(L"Arial", (float)fontSize, FontStyle::Bold);

        lblMoves->Font = newFont;
        lblCompleted->Font = newFont;
        lblCardsLeft->Font = newFont;
        lblDealsLeft->Font = newFont;
        lblStatus->Font = newFont;
    }

    void UpdatePilePositions()
    {
        if (tableauPiles == nullptr) return;

        for (int i = 0; i < tableauPiles->Count; i++)
        {
            if (tableauPiles[i] != nullptr)
            {
                int xPos = startX + i * (cardWidth + (int)(5 * scaleFactor));
                tableauPiles[i]->SetPosition(xPos, startY);
                tableauPiles[i]->SetCardSize(cardWidth, cardHeight, pileOffset);
            }
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

    void InitializeGame()
    {
        try
        {
            movesCount = 0;
            completedSuits = 0;
            dealsRemaining = 5;
            gameOver = false;

            ResetDragState();

            deck = gcnew SpiderDeck(suitsCount);
            deck->Shuffle();

            tableauPiles = gcnew List<SpiderPile^>();

            CalculateScale();

            for (int i = 0; i < 10; i++)
            {
                int xPos = startX + i * (cardWidth + (int)(5 * scaleFactor));
                SpiderPile^ pile = gcnew SpiderPile(xPos, startY, false, true);
                pile->SetCardSize(cardWidth, cardHeight, pileOffset);
                tableauPiles->Add(pile);
            }

            int totalDealt = 0;
            for (int i = 0; i < 10; i++)
            {
                int cardsCount = (i < 4) ? 6 : 5;
                for (int j = 0; j < cardsCount; j++)
                {
                    Card^ card = deck->DealCard();
                    if (card != nullptr)
                    {
                        card->SetFaceUp(j == cardsCount - 1);
                        tableauPiles[i]->AddCard(card);
                        totalDealt++;
                    }
                }
            }

            UpdateStats();
            this->Invalidate();

            lblStatus->Text = L"Новая игра! Раздано " + totalDealt.ToString() + L" карт";
        }
        catch (Exception^ ex)
        {
            MessageBox::Show("Ошибка: " + ex->Message);
        }
    }

    void ResetDragState()
    {
        isDragging = false;
        dragSourcePile = nullptr;
        dragStartIndex = -1;
        if (dragCards != nullptr)
        {
            dragCards->Clear();
            delete dragCards;
            dragCards = nullptr;
        }
        dragOffset = Point(0, 0);
    }

    void GameForm_Paint(Object^ sender, PaintEventArgs^ e)
    {
        try
        {
            Graphics^ g = e->Graphics;

            if (deck != nullptr)
            {
                String^ info = L"Колода: " + deck->GetSize().ToString() + L" карт";
                int fontSize = (int)(10 * scaleFactor);
                if (fontSize < 8) fontSize = 8;
                if (fontSize > 16) fontSize = 16;
                System::Drawing::Font^ infoFont = gcnew System::Drawing::Font(L"Arial", (float)fontSize, FontStyle::Bold);
                SolidBrush^ infoBrush = gcnew SolidBrush(Color::White);
                g->DrawString(info, infoFont, infoBrush, (float)(10 * scaleFactor), (float)infoY);
            }

            if (tableauPiles != nullptr)
            {
                for each (SpiderPile ^ pile in tableauPiles)
                {
                    if (pile != nullptr) pile->DrawPile(g, pileOffset);
                }
            }

            if (isDragging && dragCards != nullptr && dragCards->Count > 0)
            {
                int x = dragStartPoint.X - dragOffset.X;
                int y = dragStartPoint.Y - dragOffset.Y;

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

        if (gameOver)
        {
            lblStatus->Text = L"Игра окончена. Начните новую игру.";
            return;
        }

        if (isDragging)
            return;

        try
        {
            if (tableauPiles != nullptr)
            {
                for (int i = 0; i < tableauPiles->Count; i++)
                {
                    SpiderPile^ pile = tableauPiles[i];
                    if (pile == nullptr) continue;

                    int cardIndex = pile->GetCardIndexAtPosition(e->X, e->Y);
                    if (cardIndex >= 0)
                    {
                        bool allOpen = true;
                        for (int j = cardIndex; j < pile->GetCards()->Count; j++)
                        {
                            if (!pile->GetCards()[j]->IsFaceUp())
                            {
                                allOpen = false;
                                break;
                            }
                        }

                        if (!allOpen)
                        {
                            lblStatus->Text = L"Можно перетаскивать только открытые карты";
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

                            if (current->GetRank() != next->GetRank() + 1)
                            {
                                validSequence = false;
                                break;
                            }
                        }

                        if (!validSequence)
                        {
                            lblStatus->Text = L"Карты должны идти по убыванию";
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

                        Point cardPos = pile->GetCardPosition(cardIndex);
                        dragOffset = Point(e->X - cardPos.X, e->Y - cardPos.Y);
                        dragStartPoint = e->Location;

                        lblStatus->Text = L"Перетаскиваю " + dragCards->Count.ToString() + L" карт";
                        this->Invalidate();
                        return;
                    }
                }
            }
        }
        catch (Exception^ ex)
        {
            lblStatus->Text = L"Ошибка: " + ex->Message;
            ResetDragState();
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
            ResetDragState();
            this->Invalidate();
            return;
        }

        try
        {
            bool dropped = false;

            if (tableauPiles != nullptr)
            {
                for (int i = 0; i < tableauPiles->Count; i++)
                {
                    SpiderPile^ pile = tableauPiles[i];
                    if (pile == nullptr || pile == dragSourcePile) continue;

                    if (pile->IsAtPosition(e->X, e->Y))
                    {
                        Card^ firstCard = dragCards[0];
                        if (firstCard == nullptr) break;

                        if (pile->IsEmpty() || pile->CanAddCard(firstCard))
                        {
                            List<Card^>^ sourceCards = dragSourcePile->GetCards();
                            List<Card^>^ destCards = pile->GetCards();

                            for (int j = 0; j < dragCards->Count; j++)
                            {
                                sourceCards->RemoveAt(sourceCards->Count - 1);
                            }

                            for each (Card ^ card in dragCards)
                            {
                                destCards->Add(card);
                            }

                            if (!dragSourcePile->IsEmpty())
                            {
                                Card^ top = dragSourcePile->GetTopCard();
                                if (top != nullptr && !top->IsFaceUp())
                                    top->SetFaceUp(true);
                            }

                            dropped = true;
                            movesCount++;
                            lblStatus->Text = L"Карты перемещены";

                            CheckAndRemoveCompleteSequences();
                            UpdateStats();
                            this->Invalidate();
                            break;
                        }
                        else
                        {
                            lblStatus->Text = L"Сюда нельзя положить эти карты";
                        }
                    }
                }
            }

            if (!dropped)
            {
                lblStatus->Text = L"Перемещение отменено";
            }
        }
        catch (Exception^ ex)
        {
            lblStatus->Text = L"Ошибка: " + ex->Message;
        }

        ResetDragState();
        this->Invalidate();
    }

    void CheckAndRemoveCompleteSequences()
    {
        if (tableauPiles == nullptr) return;

        bool sequenceFound = true;
        while (sequenceFound)
        {
            sequenceFound = false;

            for each (SpiderPile ^ pile in tableauPiles)
            {
                if (pile == nullptr || pile->GetSize() < 13) continue;

                List<Card^>^ cards = pile->GetCards();
                int startIndex = cards->Count - 13;

                if (startIndex >= 0)
                {
                    bool isComplete = true;
                    for (int i = 0; i < 13; i++)
                    {
                        Card^ card = cards[startIndex + i];
                        if (card == nullptr || !card->IsFaceUp() || card->GetRank() != 13 - i)
                        {
                            isComplete = false;
                            break;
                        }
                    }

                    if (isComplete)
                    {
                        for (int i = 0; i < 13; i++)
                        {
                            cards->RemoveAt(cards->Count - 1);
                        }

                        completedSuits++;
                        sequenceFound = true;
                        lblStatus->Text = L"Полная последовательность удалена! (" + completedSuits.ToString() + L"/8)";

                        if (!pile->IsEmpty())
                        {
                            Card^ top = pile->GetTopCard();
                            if (top != nullptr && !top->IsFaceUp())
                                top->SetFaceUp(true);
                        }

                        CheckWin();
                        this->Invalidate();
                        break;
                    }
                }
            }
        }
    }

    void HandleDealClick()
    {
        if (gameOver)
        {
            lblStatus->Text = L"Игра окончена. Начните новую игру.";
            return;
        }

        if (deck == nullptr)
        {
            lblStatus->Text = L"Ошибка: колода не инициализирована";
            return;
        }

        if (deck->IsEmpty())
        {
            lblStatus->Text = L"Колода пуста!";
            CheckForPossibleMoves();
            return;
        }

        if (dealsRemaining <= 0)
        {
            lblStatus->Text = L"Больше нет раздач!";
            CheckForPossibleMoves();
            return;
        }

        bool hasEmptyPile = false;
        for each (SpiderPile ^ pile in tableauPiles)
        {
            if (pile != nullptr && pile->IsEmpty())
            {
                hasEmptyPile = true;
                break;
            }
        }

        if (hasEmptyPile)
        {
            lblStatus->Text = L"Нельзя сдавать карты, есть пустые стопки!";
            MessageBox::Show("Заполните все пустые стопки перед сдачей карт!",
                "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        int cardsDealt = 0;
        for each (SpiderPile ^ pile in tableauPiles)
        {
            if (pile == nullptr) continue;

            if (!deck->IsEmpty())
            {
                Card^ card = deck->DealCard();
                if (card != nullptr)
                {
                    card->SetFaceUp(true);
                    pile->AddCard(card);
                    cardsDealt++;
                }
            }
        }

        if (cardsDealt > 0)
        {
            dealsRemaining--;
            movesCount++;
            lblStatus->Text = L"Сдано " + cardsDealt.ToString() + L" карт. Осталось раздач: " + dealsRemaining.ToString();

            CheckAndRemoveCompleteSequences();
            UpdateStats();
            this->Invalidate();
        }
        else
        {
            lblStatus->Text = L"Нет карт для сдачи";
        }
    }

    void CheckForPossibleMoves()
    {
        bool hasMoves = false;

        if (tableauPiles != nullptr)
        {
            for (int i = 0; i < tableauPiles->Count && !hasMoves; i++)
            {
                SpiderPile^ sourcePile = tableauPiles[i];
                if (sourcePile == nullptr || sourcePile->IsEmpty()) continue;

                List<Card^>^ sourceCards = sourcePile->GetCards();

                for (int k = 0; k < sourceCards->Count && !hasMoves; k++)
                {
                    Card^ sourceCard = sourceCards[k];
                    if (sourceCard == nullptr || !sourceCard->IsFaceUp()) continue;

                    for (int j = 0; j < tableauPiles->Count && !hasMoves; j++)
                    {
                        if (j == i) continue;

                        SpiderPile^ destPile = tableauPiles[j];
                        if (destPile == nullptr) continue;

                        if (destPile->IsEmpty() || destPile->CanAddCard(sourceCard))
                        {
                            hasMoves = true;
                        }
                    }
                }
            }
        }

        if (!hasMoves && deck != nullptr && deck->IsEmpty() && dealsRemaining <= 0)
        {
            gameOver = true;
            lblStatus->Text = L"Игра проиграна! Нет возможных ходов.";
            MessageBox::Show("Нет возможных ходов! Игра проиграна.", "Поражение",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void CheckWin()
    {
        if (completedSuits == 8)
        {
            gameOver = true;

            // Запрос имени для рекорда
            String^ name = Microsoft::VisualBasic::Interaction::InputBox(
                L"Поздравляем! Вы выиграли за " + movesCount.ToString() + L" ходов!\nВведите ваше имя для рекорда:",
                L"Победа!",
                L"Игрок",
                -1, -1);

            if (!String::IsNullOrEmpty(name))
            {
                Records::AddRecord("Spider", name, movesCount);
            }

            MessageBox::Show("Поздравляем! Вы выиграли!", "Победа!",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
            lblStatus->Text = L"Вы выиграли! 🎉";
        }
    }

    void UpdateStats()
    {
        lblMoves->Text = L"Ходов: " + movesCount.ToString();
        lblCompleted->Text = L"Собрано: " + completedSuits.ToString() + L"/8";
        if (deck != nullptr)
            lblCardsLeft->Text = L"В колоде: " + deck->GetSize().ToString();
        lblDealsLeft->Text = L"Раздач: " + dealsRemaining.ToString();
    }

    void btnNewGame_Click(Object^ sender, EventArgs^ e)
    {
        if (MessageBox::Show("Начать новую игру?", "Подтверждение",
            MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes)
        {
            InitializeGame();
        }
    }

    void btnDeal_Click(Object^ sender, EventArgs^ e)
    {
        HandleDealClick();
    }

    void btnBack_Click(Object^ sender, EventArgs^ e)
    {
        this->Close();
    }
};