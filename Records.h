#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;

public ref class RecordEntry
{
public:
    String^ PlayerName;
    int Moves;
    DateTime Date;

    RecordEntry(String^ name, int moves)
    {
        PlayerName = name;
        Moves = moves;
        Date = DateTime::Now;
    }

    RecordEntry(String^ name, int moves, DateTime date)
    {
        PlayerName = name;
        Moves = moves;
        Date = date;
    }
};

public ref class Records
{
private:
    static String^ recordsFile = "records.txt";
    static List<RecordEntry^>^ klondikeRecords;
    static List<RecordEntry^>^ spiderRecords;
    static int maxRecords = 10;

public:
    static Records()
    {
        klondikeRecords = gcnew List<RecordEntry^>();
        spiderRecords = gcnew List<RecordEntry^>();
        LoadRecords();
    }

    static void LoadRecords()
    {
        try
        {
            if (!File::Exists(recordsFile))
                return;

            array<String^>^ lines = File::ReadAllLines(recordsFile);
            for each (String ^ line in lines)
            {
                array<String^>^ parts = line->Split(',');
                if (parts->Length == 4)
                {
                    String^ game = parts[0];
                    String^ name = parts[1];
                    int moves = Int32::Parse(parts[2]);
                    DateTime date = DateTime::Parse(parts[3]);

                    RecordEntry^ entry = gcnew RecordEntry(name, moves, date);
                    if (game == "Klondike")
                        klondikeRecords->Add(entry);
                    else if (game == "Spider")
                        spiderRecords->Add(entry);
                }
            }

            SortRecords();
        }
        catch (Exception^) {}
    }

    static void SaveRecords()
    {
        try
        {
            List<String^>^ lines = gcnew List<String^>();
            for each (RecordEntry ^ entry in klondikeRecords)
            {
                lines->Add("Klondike," + entry->PlayerName + "," + entry->Moves.ToString() + "," + entry->Date.ToString());
            }
            for each (RecordEntry ^ entry in spiderRecords)
            {
                lines->Add("Spider," + entry->PlayerName + "," + entry->Moves.ToString() + "," + entry->Date.ToString());
            }
            File::WriteAllLines(recordsFile, lines);
        }
        catch (Exception^) {}
    }

    static void SortRecords()
    {
        // Сортируем по количеству ходов (по возрастанию)
        klondikeRecords->Sort(gcnew Comparison<RecordEntry^>(&Records::CompareRecords));
        spiderRecords->Sort(gcnew Comparison<RecordEntry^>(&Records::CompareRecords));

        // Оставляем только top 10
        while (klondikeRecords->Count > maxRecords)
            klondikeRecords->RemoveAt(klondikeRecords->Count - 1);
        while (spiderRecords->Count > maxRecords)
            spiderRecords->RemoveAt(spiderRecords->Count - 1);
    }

    static int CompareRecords(RecordEntry^ a, RecordEntry^ b)
    {
        return a->Moves.CompareTo(b->Moves);
    }

    static void AddRecord(String^ game, String^ name, int moves)
    {
        RecordEntry^ entry = gcnew RecordEntry(name, moves);
        if (game == "Klondike")
        {
            klondikeRecords->Add(entry);
            SortRecords();
        }
        else if (game == "Spider")
        {
            spiderRecords->Add(entry);
            SortRecords();
        }
        SaveRecords();
    }

    static List<RecordEntry^>^ GetKlondikeRecords()
    {
        return klondikeRecords;
    }

    static List<RecordEntry^>^ GetSpiderRecords()
    {
        return spiderRecords;
    }

    static bool IsRecord(String^ game, int moves)
    {
        List<RecordEntry^>^ records = (game == "Klondike") ? klondikeRecords : spiderRecords;
        if (records->Count < maxRecords)
            return true;

        // Проверяем, меньше ли ходов чем у последнего в таблице
        RecordEntry^ last = records[records->Count - 1];
        return moves < last->Moves;
    }
};