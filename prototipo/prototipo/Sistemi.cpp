#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include "Sistemi.h"
#include "StartMenu.h"
#include "SlotManager.h"
#include "StatsGUI.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>

const char* BLOCCO_PIENO = "\xE2\x96\x88";
const char* FACCIA_PIENA = "\xE2\x98\xBB";
const char* SLIME = "\xE2\x97\x8F";
const char* ORCO = "\xF0\x9F\x91\xB9";

// =====================================================
// =====================================================

#pragma region UTILITY_CONSOLE_E_ZONE

static void StampaEntitaUTF8(
    EntityManager& Entity,
    EntityID EntityDaStampare,
    EntityID PlayerID
)
{
    if(EntityDaStampare == PlayerID)
    {
        std::cout << FACCIA_PIENA;
        return;
    }

    char Simbolo =
        Entity.visuale[EntityDaStampare].Simbolo;

    if(Simbolo == '&')
    {
        std::cout << SLIME;
    }
    else if(Simbolo == 'O')
    {
        std::cout << ORCO;
    }
    else
    {
        std::cout << Simbolo;
    }
}

void SetupUnicodeConsole()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

// =====================================================
// =====================================================

void MoveCursor(int x, int y)
{
    COORD coord;
    coord.Y = y;
    coord.X = x;

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord
    );
}

void HideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;

    SetConsoleCursorInfo(consoleHandle, &info);
}

// =====================================================
// =====================================================

void SetColor(int color)
{
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        color
    );
}


// =====================================================
// =====================================================

static int GetCimiteroCella(int x, int y)
{
    const int MIN_X = 52;
    const int MAX_X = 78;
    const int MIN_Y = 63;
    const int MAX_Y = 77;

    if(x < MIN_X || x > MAX_X || y < MIN_Y || y > MAX_Y)
    {
        return -1;
    }

    if(y == MIN_Y)
    {
        if(x >= 64 && x <= 66)
            return 26;

        return 24;
    }

    if(y == MAX_Y || x == MIN_X || x == MAX_X)
        return 24;

    if(x >= 64 && x <= 66)
        return 26;

    if(y >= 72 && y <= 76 && x >= 61 && x <= 69)
    {
        if(y == 72 && x == 65)
            return 28;

        if(y == 72 || y == 76 || x == 61 || x == 69)
            return 27;
    }

    if(
        ((y == 66 || y == 69 || y == 72) &&
         (x == 56 || x == 59 || x == 71 || x == 74)) ||
        ((y == 67 || y == 70) &&
         (x == 54 || x == 76))
    )
    {
        return 25;
    }

    return 0;
}

static bool CimiteroBlocca(int x, int y)
{
    int Tipo = GetCimiteroCella(x, y);

    return
        Tipo == 24 ||
        Tipo == 25 ||
        Tipo == 27;
}

static int GetCellaMondo(
    Map& m,
    int y,
    int x
)
{
    int Cimitero = GetCimiteroCella(x, y);

    if(Cimitero != -1 && Cimitero != 0)
        return Cimitero;

    return m.GetCella(y, x);
}

static int ContaAcquaVicino(
    Map& m,
    int PlayerX,
    int PlayerY,
    int Raggio
)
{
    int Totale = 0;

    for(int y = PlayerY - Raggio; y <= PlayerY + Raggio; y++)
    {
        for(int x = PlayerX - Raggio; x <= PlayerX + Raggio; x++)
        {
            if(x < 0 || x >= 160 || y < 0 || y >= 80)
                continue;

            if(m.GetCella(y, x) == 3)
                Totale++;
        }
    }

    return Totale;
}

static int ContaAlberiVicino(
    Map& m,
    int PlayerX,
    int PlayerY,
    int Raggio
)
{
    int Totale = 0;

    for(int y = PlayerY - Raggio; y <= PlayerY + Raggio; y++)
    {
        for(int x = PlayerX - Raggio; x <= PlayerX + Raggio; x++)
        {
            if(x < 0 || x >= 160 || y < 0 || y >= 80)
                continue;

            if(m.GetCella(y, x) == 5)
                Totale++;
        }
    }

    return Totale;
}

static bool VicinoCastello(
    Map& m,
    int PlayerX,
    int PlayerY
)
{
    for(int y = PlayerY - 4; y <= PlayerY + 4; y++)
    {
        for(int x = PlayerX - 4; x <= PlayerX + 4; x++)
        {
            if(x < 0 || x >= 160 || y < 0 || y >= 80)
                continue;

            int Cella = m.GetCella(y, x);

            if(Cella >= 9 && Cella <= 13)
                return true;
        }
    }

    return false;
}

static bool VicinoVillaggio(
    Map& m,
    int PlayerX,
    int PlayerY
)
{
    int ElementiVillaggio = 0;

    for(int y = PlayerY - 4; y <= PlayerY + 4; y++)
    {
        for(int x = PlayerX - 4; x <= PlayerX + 4; x++)
        {
            if(x < 0 || x >= 160 || y < 0 || y >= 80)
                continue;

            int Cella = m.GetCella(y, x);

            if(
                Cella == 6 || Cella == 7 || Cella == 8 ||
                (Cella >= 14 && Cella <= 23)
            )
            {
                ElementiVillaggio++;
            }
        }
    }

    return ElementiVillaggio >= 3;
}

static int ContaNemiciVicini(
    EntityManager& Entity,
    EntityID PlayerID,
    int Raggio
)
{
    int Totale = 0;

    int PlayerX = Entity.posizione[PlayerID].X;
    int PlayerY = Entity.posizione[PlayerID].Y;

    for(
        std::map<EntityID, Posizione>::iterator IT = Entity.posizione.begin();
        IT != Entity.posizione.end();
        ++IT
    )
    {
        if(IT->first == PlayerID)
            continue;

        int DX = IT->second.X - PlayerX;
        int DY = IT->second.Y - PlayerY;

        if(DX < 0) DX = -DX;
        if(DY < 0) DY = -DY;

        if(DX <= Raggio && DY <= Raggio)
            Totale++;
    }

    return Totale;
}

static void DrawZonePanel(
    Map& m,
    EntityManager& Entity,
    EntityID PlayerID,
    bool ForceRedraw
)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int X = Entity.posizione[PlayerID].X;
    int Y = Entity.posizione[PlayerID].Y;

    std::string Zona = "TERRE SELVAGGE";
    std::string Tipo = "AREA APERTA";

    if(GetCimiteroCella(X, Y) != -1)
    {
        Zona = "CIMITERO";
        Tipo = "AREA PERICOLOSA";
    }
    else if(VicinoCastello(m, X, Y))
    {
        Zona = "CASTELLO";
        Tipo = "AREA FORTIFICATA";
    }
    else if(VicinoVillaggio(m, X, Y))
    {
        Zona = "VILLAGGIO";
        Tipo = "AREA ABITATA";
    }
    else
    {
        int Acqua = ContaAcquaVicino(m, X, Y, 5);

        if(Acqua >= 30)
        {
            Zona = "LAGHETTO";
            Tipo = "RIVA DEL LAGO";
        }
        else if(Acqua >= 4)
        {
            Zona = "FIUME";
            Tipo = "RIVA DEL FIUME";
        }
        else if(ContaAlberiVicino(m, X, Y, 4) >= 8)
        {
            Zona = "BOSCO";
            Tipo = "AREA SELVAGGIA";
        }
    }

    int Nemici = ContaNemiciVicini(Entity, PlayerID, 10);

    const int SX = 99;
    const int SY = 8;

    static bool Disegnato = false;
    static std::string UltimaZona = "";
    static std::string UltimoTipo = "";
    static int UltimiNemici = -999;

    bool ZonaCambiata =
        !Disegnato ||
        Zona != UltimaZona ||
        Tipo != UltimoTipo;

    bool PresenzeCambiate =
        !Disegnato ||
        Nemici != UltimiNemici;

    if(ForceRedraw)
    {
        Disegnato = false;
        ZonaCambiata = true;
        PresenzeCambiate = true;
    }

    if(!Disegnato)
    {
        SetConsoleTextAttribute(hConsole, 11);

        MoveCursor(SX, SY);
        std::cout << "\xE2\x95\x94\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x97";

        MoveCursor(SX, SY + 1);
        std::cout << "\xE2\x95\x91            ZONA            \xE2\x95\x91";

        MoveCursor(SX, SY + 2);
        std::cout << "\xE2\x95\x9A\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x9D";

        MoveCursor(SX, SY + 8);
        std::cout << "\xE2\x95\x94\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x97";

        MoveCursor(SX, SY + 9);
        std::cout << "\xE2\x95\x91          PRESENZE          \xE2\x95\x91";

        MoveCursor(SX, SY + 10);
        std::cout << "\xE2\x95\x9A\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x9D";

        Disegnato = true;
    }

    if(ZonaCambiata)
    {
        SetConsoleTextAttribute(hConsole, 15);

        MoveCursor(SX + 2, SY + 4);
        std::cout << "                          ";
        MoveCursor(SX + 2, SY + 4);
        std::cout << Zona;

        SetConsoleTextAttribute(hConsole, 8);

        MoveCursor(SX + 2, SY + 6);
        std::cout << "                          ";
        MoveCursor(SX + 2, SY + 6);
        std::cout << Tipo;

        UltimaZona = Zona;
        UltimoTipo = Tipo;
    }

    if(PresenzeCambiate)
    {
        MoveCursor(SX + 2, SY + 12);
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "                          ";

        MoveCursor(SX + 2, SY + 12);

        if(Nemici == 0)
        {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "NESSUNA MINACCIA";
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 12);
            std::cout << "NEMICI VICINI: " << Nemici;
        }

        UltimiNemici = Nemici;
    }

    SetConsoleTextAttribute(hConsole, 7);
}

// =====================================================
// PERSONAGGI
// =====================================================

static const char* BLOCCO_CHAR = "\xE2\x96\x88";
static const char* BLOCCO_MEDIO_CHAR = "\xE2\x96\x93";
static const char* BLOCCO_CHIARO_CHAR = "\xE2\x96\x91";

#pragma endregion

#pragma region PERSONAGGI

static void DisegnaRettangoloChar(
    int X,
    int Y,
    int Larghezza,
    int Altezza,
    int Colore,
    const char* Blocco
)
{
    SetColor(Colore);

    for(int Riga = 0; Riga < Altezza; Riga++)
    {
        MoveCursor(X, Y + Riga);

        for(int Colonna = 0; Colonna < Larghezza; Colonna++)
        {
            std::cout << Blocco;
        }
    }
}

static void DisegnaLineaChar(
    int X,
    int Y,
    int Lunghezza,
    int Colore,
    const char* Blocco
)
{
    SetColor(Colore);
    MoveCursor(X, Y);

    for(int i = 0; i < Lunghezza; i++)
    {
        std::cout << Blocco;
    }
}

static void TitoloSchedaPersonaggio(
    const char* Nome,
    const char* Sottotitolo,
    int Colore
)
{
    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    MoveCursor(42, 2);
    std::cout << "ASCIIMYTHOS - PERSONAGGIO";

    SetColor(Colore);

    MoveCursor(54, 4);
    std::cout << Nome;

    SetColor(8);

    MoveCursor(43, 39);
    std::cout << Sottotitolo;

    SetColor(7);

    MoveCursor(45, 44);
    std::cout << "[P] / [ESC] TORNA AL GIOCO";
}

static void DisegnaNinja()
{
    const int NERO =
        FOREGROUND_BLUE;

    const int SCURO =
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY;

    const int AZZURRO =
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY;

    const int BIANCO =
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY;

    DisegnaRettangoloChar(
        45, 10, 37, 24,
        8,
        BLOCCO_CHIARO_CHAR
    );

    DisegnaRettangoloChar(
        55, 28, 7, 9,
        SCURO,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        68, 28, 7, 9,
        SCURO,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        52, 17, 26, 13,
        SCURO,
        BLOCCO_CHAR
    );

    DisegnaRettangoloChar(
        52, 26, 26, 2,
        AZZURRO,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        46, 19, 6, 10,
        SCURO,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        78, 19, 6, 10,
        SCURO,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        55, 8, 20, 10,
        SCURO,
        BLOCCO_CHAR
    );

    DisegnaLineaChar(
        58, 12, 14,
        8,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        58, 13, 14, 3,
        8,
        BLOCCO_CHAR
    );

    SetColor(AZZURRO);
    MoveCursor(61, 14);
    std::cout << "[]";

    MoveCursor(68, 14);
    std::cout << "[]";

    DisegnaRettangoloChar(
        59, 16, 12, 2,
        NERO,
        BLOCCO_MEDIO_CHAR
    );

    SetColor(BIANCO);

    for(int i = 0; i < 19; i++)
    {
        MoveCursor(
            80 - (i / 2),
            8 + i
        );

        std::cout << "/";
    }

    SetColor(AZZURRO);
    MoveCursor(70, 25);
    std::cout << "====";

    SetColor(AZZURRO);
    MoveCursor(75, 10);
    std::cout << ">>>>";

    MoveCursor(76, 11);
    std::cout << ">>>>>>";

    TitoloSchedaPersonaggio(
        "NINJA",
        "Rapido, silenzioso, addestrato per colpire dall'ombra.",
        AZZURRO
    );
}

static void DisegnaCavaliere()
{
    const int ACCIAIO =
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY;

    const int OMBRA =
        FOREGROUND_INTENSITY;

    const int ORO =
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_INTENSITY;

    const int ROSSO =
        FOREGROUND_RED |
        FOREGROUND_INTENSITY;

    DisegnaRettangoloChar(
        44, 9, 40, 27,
        8,
        BLOCCO_CHIARO_CHAR
    );

    DisegnaRettangoloChar(
        55, 28, 8, 9,
        ACCIAIO,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        68, 28, 8, 9,
        ACCIAIO,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        51, 17, 28, 13,
        ACCIAIO,
        BLOCCO_CHAR
    );

    DisegnaRettangoloChar(
        55, 20, 20, 7,
        OMBRA,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        64, 20, 2, 7,
        ORO,
        BLOCCO_CHAR
    );

    DisegnaRettangoloChar(
        60, 22, 10, 2,
        ORO,
        BLOCCO_CHAR
    );

    DisegnaRettangoloChar(
        55, 8, 21, 11,
        ACCIAIO,
        BLOCCO_CHAR
    );

    DisegnaRettangoloChar(
        57, 13, 17, 3,
        OMBRA,
        BLOCCO_MEDIO_CHAR
    );

    SetColor(7);
    MoveCursor(59, 14);
    std::cout << "- - - - - - -";

    DisegnaRettangoloChar(
        64, 5, 3, 4,
        ROSSO,
        BLOCCO_MEDIO_CHAR
    );

    MoveCursor(67, 6);
    SetColor(ROSSO);
    std::cout << ">>>>";

    DisegnaRettangoloChar(
        42, 20, 9, 12,
        ORO,
        BLOCCO_MEDIO_CHAR
    );

    SetColor(ROSSO);
    MoveCursor(45, 23);
    std::cout << "+";

    MoveCursor(44, 24);
    std::cout << "+++";

    MoveCursor(45, 25);
    std::cout << "+";

    SetColor(ACCIAIO);

    for(int i = 0; i < 20; i++)
    {
        MoveCursor(83, 10 + i);
        std::cout << "|";
    }

    SetColor(ORO);
    MoveCursor(79, 28);
    std::cout << "========";

    MoveCursor(82, 30);
    std::cout << "[]";

    TitoloSchedaPersonaggio(
        "CAVALIERE",
        "Armatura pesante, disciplina e forza sul campo di battaglia.",
        ORO
    );
}

static void DisegnaMago()
{
    const int VIOLA =
        FOREGROUND_RED |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY;

    const int BLU =
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY;

    const int CELESTE =
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY;

    const int ORO =
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_INTENSITY;

    const int BIANCO =
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY;

    DisegnaRettangoloChar(
        43, 9, 42, 28,
        BLU,
        BLOCCO_CHIARO_CHAR
    );

    DisegnaRettangoloChar(
        53, 20, 25, 15,
        VIOLA,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        49, 31, 33, 5,
        VIOLA,
        BLOCCO_CHAR
    );

    DisegnaRettangoloChar(
        58, 13, 16, 8,
        ORO,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        60, 15, 12, 4,
        8,
        BLOCCO_CHAR
    );

    SetColor(CELESTE);
    MoveCursor(62, 16);
    std::cout << "<>";

    MoveCursor(68, 16);
    std::cout << "<>";

    DisegnaRettangoloChar(
        51, 11, 30, 3,
        VIOLA,
        BLOCCO_CHAR
    );

    DisegnaRettangoloChar(
        57, 8, 19, 3,
        VIOLA,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        61, 6, 13, 2,
        VIOLA,
        BLOCCO_MEDIO_CHAR
    );

    DisegnaRettangoloChar(
        67, 4, 6, 2,
        VIOLA,
        BLOCCO_CHAR
    );

    SetColor(ORO);
    MoveCursor(60, 9);
    std::cout << "*";

    MoveCursor(69, 7);
    std::cout << "*";

    MoveCursor(73, 10);
    std::cout << "*";

    SetColor(ORO);

    for(int i = 0; i < 23; i++)
    {
        MoveCursor(86, 12 + i);
        std::cout << "|";
    }

    SetColor(CELESTE);
    MoveCursor(84, 8);
    std::cout << " /\\ ";

    MoveCursor(83, 9);
    std::cout << "<()>";

    MoveCursor(84, 10);
    std::cout << " \\/ ";

    SetColor(BIANCO);
    MoveCursor(91, 7);
    std::cout << "*";

    MoveCursor(89, 12);
    std::cout << "+";

    MoveCursor(95, 10);
    std::cout << "*";

    TitoloSchedaPersonaggio(
        "MAGO",
        "Studioso delle arti arcane, domina energia e incantesimi.",
        VIOLA
    );
}

static void DisegnaSchedaPersonaggio(
    EntityManager& Entity,
    EntityID PlayerID
)
{
    system("cls");

    char Classe =
        Entity.visuale[PlayerID].Simbolo;

    if(Classe == 'N')
    {
        DisegnaNinja();
    }
    else if(Classe == 'C')
    {
        DisegnaCavaliere();
    }
    else
    {
        DisegnaMago();
    }
}

void MostraPersonaggio(
    EntityManager& Entity,
    EntityID PlayerID
)
{
    DisegnaSchedaPersonaggio(
        Entity,
        PlayerID
    );

    bool Aperto = true;

    while(Aperto)
    {
        if(_kbhit())
        {
            char Tasto =
                _getch();

            if(
                Tasto == 'p' ||
                Tasto == 'P' ||
                Tasto == 27
            )
            {
                Aperto = false;
            }
        }

        Sleep(20);
    }
}

void SelezionaPersonaggio(
    EntityManager& Entity,
    EntityID PlayerID
)
{
    int Scelta = 1;
    bool Confermato = false;
    bool Ridisegna = true;

    while(!Confermato)
    {
        if(Ridisegna)
        {
            system("cls");

            SetColor(11);
            MoveCursor(43, 4);
            std::cout << "SCEGLI IL TUO PERSONAGGIO";

            SetColor(8);
            MoveCursor(31, 7);
            std::cout << "La scelta viene salvata nello slot e potrai vedere il personaggio con [P].";

            if(Scelta == 1) SetColor(11);
            else SetColor(8);

            MoveCursor(20, 13);
            std::cout << "+----------------------+";
            MoveCursor(20, 14);
            std::cout << "|        NINJA         |";
            MoveCursor(20, 15);
            std::cout << "+----------------------+";

            SetColor(7);
            MoveCursor(20, 17);
            std::cout << "Maschera + katana";
            MoveCursor(20, 18);
            std::cout << "Stile: furtivo";

            if(Scelta == 2) SetColor(14);
            else SetColor(8);

            MoveCursor(53, 13);
            std::cout << "+----------------------+";
            MoveCursor(53, 14);
            std::cout << "|      CAVALIERE       |";
            MoveCursor(53, 15);
            std::cout << "+----------------------+";

            SetColor(7);
            MoveCursor(53, 17);
            std::cout << "Armatura + scudo";
            MoveCursor(53, 18);
            std::cout << "Stile: resistente";

            if(Scelta == 3) SetColor(13);
            else SetColor(8);

            MoveCursor(86, 13);
            std::cout << "+----------------------+";
            MoveCursor(86, 14);
            std::cout << "|         MAGO         |";
            MoveCursor(86, 15);
            std::cout << "+----------------------+";

            SetColor(7);
            MoveCursor(86, 17);
            std::cout << "Bastone + magia";
            MoveCursor(86, 18);
            std::cout << "Stile: arcano";

            SetColor(15);
            MoveCursor(37, 26);
            std::cout << "[A/D] CAMBIA      [INVIO] CONFERMA";

            SetColor(8);
            MoveCursor(43, 29);
            std::cout << "La classe per ora e' estetica.";

            SetColor(7);

            Ridisegna = false;
        }

        if(_kbhit())
        {
            char Tasto =
                _getch();

            int VecchiaScelta =
                Scelta;

            if(
                Tasto == 'a' ||
                Tasto == 'A'
            )
            {
                Scelta--;

                if(Scelta < 1)
                    Scelta = 3;
            }
            else if(
                Tasto == 'd' ||
                Tasto == 'D'
            )
            {
                Scelta++;

                if(Scelta > 3)
                    Scelta = 1;
            }
            else if(Tasto == '1')
            {
                Scelta = 1;
            }
            else if(Tasto == '2')
            {
                Scelta = 2;
            }
            else if(Tasto == '3')
            {
                Scelta = 3;
            }
            else if(Tasto == 13)
            {
                Confermato = true;
            }

            if(
                !Confermato &&
                Scelta != VecchiaScelta
            )
            {
                Ridisegna = true;
            }
        }

        Sleep(20);
    }

    if(Scelta == 1)
    {
        Entity.visuale[PlayerID].Simbolo = 'N';
    }
    else if(Scelta == 2)
    {
        Entity.visuale[PlayerID].Simbolo = 'C';
    }
    else
    {
        Entity.visuale[PlayerID].Simbolo = 'M';
    }

    MostraPersonaggio(
        Entity,
        PlayerID
    );
}

// =====================================================
// MOVIMENTO
// =====================================================

#pragma endregion

#pragma region MOVIMENTO_E_MAPPA

void MuoviEntita(
    Map& m,
    EntityManager& Entity,
    EntityID& ID,
    char Tasto
)
{
    int y = Entity.posizione[ID].Y;
    int x = Entity.posizione[ID].X;

    if(Tasto == 'w')
    {
        if(y > 0)
        {
            int CellaDestinazione = GetCellaMondo(m, y - 1, x);

            if(CellaDestinazione != 1 &&
               CellaDestinazione != 3 &&
               CellaDestinazione != 4 &&
               CellaDestinazione != 5 &&
               CellaDestinazione != 6 &&
               CellaDestinazione != 7 &&
               CellaDestinazione != 8 &&
               CellaDestinazione != 9 &&
               CellaDestinazione != 10 &&
               CellaDestinazione != 11 &&
               CellaDestinazione != 12 &&
               CellaDestinazione != 24 &&
               CellaDestinazione != 25 &&
               CellaDestinazione != 27)
            {
                y--;
            }
        }
    }

    if(Tasto == 's')
    {
        if(y < 79)
        {
            int CellaDestinazione = GetCellaMondo(m, y + 1, x);

            if(CellaDestinazione != 1 &&
               CellaDestinazione != 3 &&
               CellaDestinazione != 4 &&
               CellaDestinazione != 5 &&
               CellaDestinazione != 6 &&
               CellaDestinazione != 7 &&
               CellaDestinazione != 8 &&
               CellaDestinazione != 9 &&
               CellaDestinazione != 10 &&
               CellaDestinazione != 11 &&
               CellaDestinazione != 12 &&
               CellaDestinazione != 24 &&
               CellaDestinazione != 25 &&
               CellaDestinazione != 27)
            {
                y++;
            }
        }
    }

    if(Tasto == 'a')
    {
        if(x > 0)
        {
            int CellaDestinazione = GetCellaMondo(m, y, x - 1);

            if(CellaDestinazione != 1 &&
               CellaDestinazione != 3 &&
               CellaDestinazione != 4 &&
               CellaDestinazione != 5 &&
               CellaDestinazione != 6 &&
               CellaDestinazione != 7 &&
               CellaDestinazione != 8 &&
               CellaDestinazione != 9 &&
               CellaDestinazione != 10 &&
               CellaDestinazione != 11 &&
               CellaDestinazione != 12 &&
               CellaDestinazione != 24 &&
               CellaDestinazione != 25 &&
               CellaDestinazione != 27)
            {
                x--;
            }
        }
    }

    if(Tasto == 'd')
    {
        if(x < 159)
        {
            int CellaDestinazione = GetCellaMondo(m, y, x + 1);

            if(CellaDestinazione != 1 &&
               CellaDestinazione != 3 &&
               CellaDestinazione != 4 &&
               CellaDestinazione != 5 &&
               CellaDestinazione != 6 &&
               CellaDestinazione != 7 &&
               CellaDestinazione != 8 &&
               CellaDestinazione != 9 &&
               CellaDestinazione != 10 &&
               CellaDestinazione != 11 &&
               CellaDestinazione != 12 &&
               CellaDestinazione != 24 &&
               CellaDestinazione != 25 &&
               CellaDestinazione != 27)
            {
                x++;
            }
        }
    }

    Entity.posizione[ID].Y = y;
    Entity.posizione[ID].X = x;
}	

// =====================================================
// =====================================================

void DrawLegend()
{
    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);


    // =====================================================
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        11
    );

    MoveCursor(2, 3);

    std::cout
        << "ASCIIMYTHOS";


    // =====================================================
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        15
    );

    MoveCursor(2, 6);

    std::cout
        << "COMANDI";


    SetConsoleTextAttribute(
        hConsole,
        8
    );


    MoveCursor(2, 8);

    std::cout
        << "[W] SU";


    MoveCursor(2, 9);

    std::cout
        << "[S] GIU";


    MoveCursor(2, 10);

    std::cout
        << "[A] SINISTRA";


    MoveCursor(2, 11);

    std::cout
        << "[D] DESTRA";


    MoveCursor(2, 13);

    std::cout
        << "[M] STATISTICHE";


    MoveCursor(2, 14);

    std::cout
        << "[P] PERSONAGGIO";


    MoveCursor(2, 15);

    std::cout
        << "[ESC] MENU";


    // =====================================================
    // =====================================================

    SetConsoleTextAttribute(
        hConsole,
        15
    );

    MoveCursor(2, 17);

    std::cout
        << "LEGENDA";


    // PLAYER
    SetConsoleTextAttribute(
        hConsole,
        14
    );

    MoveCursor(2, 19);

    std::cout
        << FACCIA_PIENA;

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  PLAYER";


    SetConsoleTextAttribute(
        hConsole,
        FOREGROUND_GREEN |
        FOREGROUND_INTENSITY
    );

    MoveCursor(2, 20);

    std::cout
        << "\xE2\x99\xA3";

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  ALBERO";


    SetConsoleTextAttribute(
        hConsole,
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    MoveCursor(2, 21);

    std::cout
        << "\xE2\x96\x91";

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  ACQUA";


    SetConsoleTextAttribute(
        hConsole,
        8
    );

    MoveCursor(2, 22);

    std::cout
        << "\xE2\x97\x86";

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  ROCCIA";


    SetConsoleTextAttribute(
        hConsole,
        FOREGROUND_RED |
        FOREGROUND_GREEN
    );

    MoveCursor(2, 23);

    std::cout
        << "\xE2\x96\x88";

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  CASA";


    SetConsoleTextAttribute(
        hConsole,
        8
    );

    MoveCursor(2, 24);

    std::cout
        << "\xE2\x96\x93";

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  TORRE";


    SetConsoleTextAttribute(
        hConsole,
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    MoveCursor(2, 25);

    std::cout
        << "\xE2\x96\x91";

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  POZZO";


    SetConsoleTextAttribute(
        hConsole,
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_INTENSITY
    );

    MoveCursor(2, 26);

    std::cout
        << "\xE2\x9C\xBF";

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  FIORI";


    SetConsoleTextAttribute(
        hConsole,
        FOREGROUND_RED |
        FOREGROUND_GREEN
    );

    MoveCursor(2, 27);

    std::cout
        << "=";

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  PANCHINA";


    SetConsoleTextAttribute(
        hConsole,
        FOREGROUND_RED |
        FOREGROUND_INTENSITY
    );

    MoveCursor(2, 28);

    std::cout
        << "\xE2\x96\x93";

    SetConsoleTextAttribute(
        hConsole,
        7
    );

    std::cout
        << "  MERCATO";


    SetConsoleTextAttribute(
        hConsole,
        7
    );
}

void MapRedering(
    Map& m,
    EntityManager& Entity,
    EntityID& ID
)
{
    const int MAP_WIDTH = 160;
    const int MAP_HEIGHT = 80;

    const int VIEW_WIDTH = 40;
    const int VIEW_HEIGHT = 24;

    const int SCREEN_X = 50;
    const int SCREEN_Y = 5;

    int PlayerX = Entity.posizione[ID].X;
    int PlayerY = Entity.posizione[ID].Y;

    int StartX = PlayerX - (VIEW_WIDTH / 2);
    int StartY = PlayerY - (VIEW_HEIGHT / 2);

    if(StartX < 0)
        StartX = 0;

    if(StartX > MAP_WIDTH - VIEW_WIDTH)
        StartX = MAP_WIDTH - VIEW_WIDTH;

    if(StartY < 0)
        StartY = 0;

    if(StartY > MAP_HEIGHT - VIEW_HEIGHT)
        StartY = MAP_HEIGHT - VIEW_HEIGHT;


    // =====================================================
    // =====================================================

    MoveCursor(
        SCREEN_X,
        SCREEN_Y
    );

    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    std::cout << "\xE2\x95\x94";

    for(int i = 0; i < VIEW_WIDTH; i++)
    {
        std::cout << "\xE2\x95\x90";
    }

    std::cout << "\xE2\x95\x97";


    // =====================================================
    // MAPPA
    // =====================================================

    for(int i = 0; i < VIEW_HEIGHT; i++)
    {
        MoveCursor(
            SCREEN_X,
            SCREEN_Y + 1 + i
        );

        SetColor(
            FOREGROUND_RED |
            FOREGROUND_GREEN |
            FOREGROUND_BLUE |
            FOREGROUND_INTENSITY
        );

        std::cout << "\xE2\x95\x91";

        int WorldY = StartY + i;


        for(int j = 0; j < VIEW_WIDTH; j++)
        {
            int WorldX = StartX + j;

            bool EntityFound = false;


            // =====================================================
            // =====================================================

            for(
                std::map<EntityID, Posizione>::iterator IT =
                Entity.posizione.begin();

                IT != Entity.posizione.end();

                ++IT
            )
            {
                if(
                    IT->second.X == WorldX &&
                    IT->second.Y == WorldY
                )
                {
                    if(IT->first == ID)
                    {
                        SetColor(
                            FOREGROUND_RED |
                            FOREGROUND_GREEN |
                            FOREGROUND_INTENSITY
                        );

                        StampaEntitaUTF8(Entity, ID, ID);
                    }
                    else
                    {
                        SetColor(
                            FOREGROUND_RED |
                            FOREGROUND_INTENSITY
                        );

                        StampaEntitaUTF8(
                            Entity,
                            IT->first,
                            ID
                        );
                    }

                    EntityFound = true;
                    break;
                }
            }


            if(EntityFound)
            {
                continue;
            }


            // =====================================================
            // =====================================================

            int Cella =
                GetCellaMondo(
                    m,
                    WorldY,
                    WorldX
                );


            switch(Cella)
            {
                // =================================================
                // =================================================

                case 0:

                    SetColor(
                        FOREGROUND_GREEN |
                        FOREGROUND_BLUE
                    );

                    std::cout << " ";

                    break;


                // =================================================
                // =================================================

                case 1:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_BLUE |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x96\x88";

                    break;


                // =================================================
                // =================================================

                case 2:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x96\x92";

                    break;


                // =================================================
                // =================================================

                case 3:

                    SetColor(
                        FOREGROUND_BLUE |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x96\x91";

                    break;


                // =================================================
                // =================================================

                case 4:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_BLUE |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x97\x86";

                    break;


                // =================================================
                // =================================================

                case 5:

                    SetColor(
                        FOREGROUND_GREEN |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x99\xA3";

                    break;


                // =================================================
                // =================================================

                case 6:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x96\x88";

                    break;


                // =================================================
                // =================================================

                case 7:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x96\x93";

                    break;


                // =================================================
                // =================================================

                case 8:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN
                    );

                    std::cout
                        << "\xE2\x96\x88";

                    break;


                // =================================================
                // =================================================

                case 9:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_BLUE |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x96\x88";

                    break;


                // =================================================
                // =================================================

                case 10:

                    SetColor(
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x96\x93";

                    break;


                // =================================================
                // =================================================

                case 11:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_BLUE |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x96\x88";

                    break;


                // =================================================
                // =================================================

                case 12:

                    SetColor(
                        FOREGROUND_BLUE |
                        FOREGROUND_GREEN |
                        FOREGROUND_INTENSITY
                    );

                    std::cout
                        << "\xE2\x96\x91";

                    break;


                // =================================================
                // =================================================

                case 13:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN
                    );

                    std::cout
                        << "\xE2\x96\x88";

                    break;


               case 14:
    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE
    );

    std::cout << "\xE2\x96\x91";
    break;


case 15:
    SetColor(
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    std::cout << "\xE2\x96\x91";
    break;

    std::cout << "\xE2\x97\x8B";
    break;


case 16:
    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN
    );

    std::cout << "\xE2\x96\x93";
    break;


case 17:
    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_INTENSITY
    );

    std::cout << "\xE2\x9C\xBF";
    break;


case 18:
    SetColor(
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    std::cout << "\xE2\x96\x91";
    break;


case 19:
    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_INTENSITY
    );

    std::cout << "\xE2\x96\x88";
    break;


case 20:
    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN
    );

    std::cout << "\xE2\x96\x92";
    break;


case 21:
    SetColor(
        FOREGROUND_RED |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    std::cout << "\xE2\x96\x93";
    break;


case 22:
    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_INTENSITY
    );

    std::cout << "\xE2\x96\x88";
    break;


case 23:
    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE
    );

    std::cout << "\xE2\x96\x93";
    break;



                case 24:
                    SetColor(8);
                    std::cout << "\xE2\x96\x93"; // ▓
                    break;

                case 25:
                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_BLUE
                    );
                    std::cout << "\xE2\x80\xA0"; // †
                    break;

                case 26:
                    SetColor(8);
                    std::cout << "\xE2\x96\x91"; // ░
                    break;

                case 27:
                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_BLUE |
                        FOREGROUND_INTENSITY
                    );
                    std::cout << "\xE2\x96\x88"; // █
                    break;

                case 28:
                    SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
                    std::cout << "\xE2\x96\x92"; // ▒
                    break;
                default:

                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_BLUE
                    );

                    std::cout << " ";

                    break;
            }
        }


        // =====================================================
        // =====================================================

        SetColor(
            FOREGROUND_RED |
            FOREGROUND_GREEN |
            FOREGROUND_BLUE |
            FOREGROUND_INTENSITY
        );

        std::cout
            << "\xE2\x95\x91";
    }


    // =====================================================
    // =====================================================

    MoveCursor(
        SCREEN_X,
        SCREEN_Y + VIEW_HEIGHT + 1
    );

    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    std::cout
        << "\xE2\x95\x9A";

    for(int i = 0; i < VIEW_WIDTH; i++)
    {
        std::cout
            << "\xE2\x95\x90";
    }

    std::cout
        << "\xE2\x95\x9D";


    // =====================================================
    // =====================================================

    DrawLegend();

    DrawZonePanel(
        m,
        Entity,
        ID,
        true
    );


    // =====================================================
    // =====================================================

    MoveCursor(
        SCREEN_X,
        SCREEN_Y + VIEW_HEIGHT + 3
    );

    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    std::cout
        << "HP: "
        << Entity.salute[ID].HP
        << "    ";

    std::cout
        << "ATK: "
        << Entity.attacco[ID].Damage
        << "    ";

    std::cout
        << "POS: "
        << PlayerX
        << ","
        << PlayerY
        << "     ";


    MoveCursor(
        SCREEN_X,
        SCREEN_Y + VIEW_HEIGHT + 4
    );

    std::cout
        << "WASD: Muovi     ESC: Menu";
}

// =====================================================
// =====================================================

int CollisionDetection(
    EntityManager& Entity,
    EntityID& ID
)
{
    int PlayerX = Entity.posizione[ID].X;
    int PlayerY = Entity.posizione[ID].Y;

    for(
        std::map<EntityID, Posizione>::iterator IT =
        Entity.posizione.begin();

        IT != Entity.posizione.end();

        ++IT
    )
    {
        EntityID OtherID = IT->first;

        if(OtherID == ID)
        {
            continue;
        }

        if(
            IT->second.X == PlayerX &&
            IT->second.Y == PlayerY
        )
        {
            return OtherID;
        }
    }

    return -1;
}

// =====================================================
// =====================================================

#pragma endregion

#pragma region NEMICI_E_RESPAWN

static bool TileSpawnabile(
    Map& m,
    int x,
    int y
)
{
    if(x < 1 || x >= 159 || y < 1 || y >= 79)
    {
        return false;
    }

    if(GetCimiteroCella(x, y) != -1)
    {
        return false;
    }

    return m.GetCella(y, x) == 0;
}

static bool PosizioneOccupata(
    EntityManager& Entity,
    int x,
    int y,
    EntityID IgnoraID
)
{
    for(
        std::map<EntityID, Posizione>::iterator IT =
            Entity.posizione.begin();
        IT != Entity.posizione.end();
        ++IT
    )
    {
        if(IT->first == IgnoraID)
        {
            continue;
        }

        if(IT->second.X == x && IT->second.Y == y)
        {
            return true;
        }
    }

    return false;
}

void SpawnNemicoCasuale(
    Map& m,
    EntityManager& Entity,
    EntityID PlayerID,
    EntityID EnemyID
)
{
    const int MAP_WIDTH = 160;
    const int MAP_HEIGHT = 80;

    for(int Tentativo = 0; Tentativo < 2000; Tentativo++)
    {
        int x = 1 + (rand() % (MAP_WIDTH - 2));
        int y = 1 + (rand() % (MAP_HEIGHT - 2));

        if(!TileSpawnabile(m, x, y))
        {
            continue;
        }

        if(PosizioneOccupata(Entity, x, y, EnemyID))
        {
            continue;
        }

        int dx = x - Entity.posizione[PlayerID].X;
        int dy = y - Entity.posizione[PlayerID].Y;

        if(dx < 0) dx = -dx;
        if(dy < 0) dy = -dy;

        if(dx < 12 && dy < 12)
        {
            continue;
        }

        Entity.posizione[EnemyID].X = x;
        Entity.posizione[EnemyID].Y = y;

        Entity.RespawnTImer[EnemyID].ReX = x;
        Entity.RespawnTImer[EnemyID].ReY = y;
        Entity.RespawnTImer[EnemyID].timer = 0;

        return;
    }
}

void GestisciRespawnNemici(
    Map& m,
    EntityManager& Entity,
    EntityID PlayerID,
    EntityID BossID
)
{
    for(
        std::map<EntityID, EntityRespawn>::iterator IT =
            Entity.RespawnTImer.begin();
        IT != Entity.RespawnTImer.end();
        ++IT
    )
    {
        EntityID EnemyID = IT->first;

        if(EnemyID == PlayerID || EnemyID == BossID)
        {
            continue;
        }

        if(IT->second.timer > 0)
        {
            IT->second.timer--;

            if(IT->second.timer == 0)
            {
                Entity.salute[EnemyID].HP =
                    Entity.salute[EnemyID].MaxHP;

                SpawnNemicoCasuale(
                    m,
                    Entity,
                    PlayerID,
                    EnemyID
                );
            }
        }
    }
}

static void DrawProgressioneNemici(
    int SlimeKills,
    bool BossSpawned,
    bool BossAlive
)
{
    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);

    MoveCursor(2, 31);

    SetConsoleTextAttribute(
        hConsole,
        15
    );

    std::cout << "OBIETTIVO                     ";

    MoveCursor(2, 32);

    if(!BossSpawned)
    {
        SetConsoleTextAttribute(
            hConsole,
            10
        );

        std::cout
            << "SLIME: "
            << SlimeKills
            << "/3                    ";
    }
    else if(BossAlive)
    {
        SetConsoleTextAttribute(
            hConsole,
            12
        );

        std::cout
            << "BOSS: ORCO ATTIVO            ";
    }
    else
    {
        SetConsoleTextAttribute(
            hConsole,
            14
        );

        std::cout
            << "ORCO SCONFITTO!              ";
    }

    SetConsoleTextAttribute(
        hConsole,
        7
    );
}


// =====================================================
// =====================================================

#pragma endregion

#pragma region COMBATTIMENTO

static void DrawCombatBar(
    int x,
    int y,
    double valore,
    double massimo,
    int larghezza,
    WORD colore
)
{
    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);

    if(massimo <= 0)
    {
        massimo = 1;
    }

    if(valore < 0)
    {
        valore = 0;
    }

    if(valore > massimo)
    {
        valore = massimo;
    }

    int pieni =
        (int)((valore / massimo) * larghezza);


    MoveCursor(x, y);


    SetConsoleTextAttribute(
        hConsole,
        colore
    );


    for(int i = 0; i < pieni; i++)
    {
        std::cout
            << "\xE2\x96\x88";
    }


    SetConsoleTextAttribute(
        hConsole,
        8
    );


    for(int i = pieni; i < larghezza; i++)
    {
        std::cout
            << "\xE2\x96\x91";
    }


    SetConsoleTextAttribute(
        hConsole,
        7
    );
}


// =====================================================
// =====================================================

static void DrawCombatFrame()
{
    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);


    SetConsoleTextAttribute(
        hConsole,
        11
    );


    MoveCursor(51, 2);

    std::cout
        << "ASCIIMYTHOS";


    MoveCursor(54, 3);

    std::cout
        << "BATTLE";


    SetConsoleTextAttribute(
        hConsole,
        8
    );


    // =================================================
    // =================================================

    MoveCursor(8, 5);

    std::cout
        << "\xE2\x95\x94";


    for(int i = 0; i < 108; i++)
    {
        std::cout
            << "\xE2\x95\x90";
    }


    std::cout
        << "\xE2\x95\x97";


    // =================================================
    // =================================================

    for(int y = 6; y <= 31; y++)
    {
        MoveCursor(8, y);

        std::cout
            << "\xE2\x95\x91";


        MoveCursor(117, y);

        std::cout
            << "\xE2\x95\x91";
    }


    // =================================================
    // =================================================

    MoveCursor(8, 22);

    std::cout
        << "\xE2\x95\xA0";


    for(int i = 0; i < 108; i++)
    {
        std::cout
            << "\xE2\x95\x90";
    }


    std::cout
        << "\xE2\x95\xA3";


    // =================================================
    // =================================================

    MoveCursor(8, 32);

    std::cout
        << "\xE2\x95\x9A";


    for(int i = 0; i < 108; i++)
    {
        std::cout
            << "\xE2\x95\x90";
    }


    std::cout
        << "\xE2\x95\x9D";


    SetConsoleTextAttribute(
        hConsole,
        7
    );
}


// =====================================================
// =====================================================

static void DrawCombatGUI(
    EntityManager& Entity,
    EntityID PlayerID,
    EntityID EnemyID,
    int BarTime,
    int EnemyBarTime,
    const std::string& BattleMessage
)
{
    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);


    // =================================================
    // PLAYER
    // =================================================

    SetConsoleTextAttribute(
        hConsole,
        11
    );


    MoveCursor(20, 8);

    std::cout
        << "PLAYER ";
    StampaEntitaUTF8(
        Entity,
        PlayerID,
        PlayerID
    );


    SetConsoleTextAttribute(
        hConsole,
        15
    );


    MoveCursor(14, 11);

    std::cout
        << "HP";


    DrawCombatBar(
        19,
        11,
        Entity.salute[PlayerID].HP,
        Entity.salute[PlayerID].MaxHP,
        24,
        10
    );


    MoveCursor(45, 11);

    std::cout
        << (int)Entity.salute[PlayerID].HP
        << "/"
        << (int)Entity.salute[PlayerID].MaxHP
        << "     ";


    MoveCursor(14, 14);

    std::cout
        << "ATB";


    DrawCombatBar(
        19,
        14,
        BarTime,
        100,
        24,
        11
    );


    MoveCursor(45, 14);

    std::cout
        << BarTime
        << "%    ";


    MoveCursor(14, 17);

    std::cout
        << "ATK: "
        << Entity.attacco[PlayerID].Damage
        << "     ";


    // =================================================
    // =================================================

    SetConsoleTextAttribute(
        hConsole,
        14
    );


    MoveCursor(60, 13);

    std::cout
        << "VS";


    // =================================================
    // =================================================

    SetConsoleTextAttribute(
        hConsole,
        12
    );


    MoveCursor(86, 8);

    std::cout
        << "NEMICO ";
    StampaEntitaUTF8(
        Entity,
        EnemyID,
        PlayerID
    );


    SetConsoleTextAttribute(
        hConsole,
        15
    );


    MoveCursor(72, 11);

    std::cout
        << "HP";


    DrawCombatBar(
        77,
        11,
        Entity.salute[EnemyID].HP,
        Entity.salute[EnemyID].MaxHP,
        24,
        12
    );


    MoveCursor(103, 11);

    std::cout
        << (int)Entity.salute[EnemyID].HP
        << "/"
        << (int)Entity.salute[EnemyID].MaxHP
        << "     ";


    MoveCursor(72, 14);

    std::cout
        << "ATB";


    DrawCombatBar(
        77,
        14,
        EnemyBarTime,
        100,
        24,
        14
    );


    MoveCursor(103, 14);

    std::cout
        << EnemyBarTime
        << "%    ";


    MoveCursor(72, 17);

    std::cout
        << "ATK: "
        << Entity.attacco[EnemyID].Damage
        << "     ";


    // =================================================
    // =================================================

    SetConsoleTextAttribute(
        hConsole,
        14
    );


    MoveCursor(14, 24);

    std::cout
        << "BATTLE LOG";


    SetConsoleTextAttribute(
        hConsole,
        7
    );


    MoveCursor(14, 26);

    std::cout
        << "                                                                                              ";


    MoveCursor(14, 26);

    std::cout
        << "> "
        << BattleMessage;
}


// =====================================================
// =====================================================

static void DrawCombatMoves(
    EntityManager& Entity,
    EntityID PlayerID
)
{
    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);


    SetConsoleTextAttribute(
        hConsole,
        14
    );


    MoveCursor(14, 28);

    std::cout
        << "SCEGLI MOSSA:";


    const std::vector<Skill>& ListaMosse =
        Entity.skillset[PlayerID].skillset;


    int x =
        32;


    for(
        size_t i = 0;
        i < ListaMosse.size();
        i++
    )
    {
        SetConsoleTextAttribute(
            hConsole,
            15
        );


        MoveCursor(
            x,
            28
        );


        std::cout
            << "["
            << i + 1
            << "] "
            << ListaMosse[i].Name
            << "  ";


        SetConsoleTextAttribute(
            hConsole,
            12
        );


        std::cout
            << ListaMosse[i].Danno +
               (int)Entity.attacco[PlayerID].Damage
            << " DMG";


        x += 27;
    }


    SetConsoleTextAttribute(
        hConsole,
        7
    );
}
// =====================================================
// COMBATTIMENTO
// =====================================================

void LevelUp(
    EntityManager& Entity,
    EntityID PlayerID
)
{
    if(
        Entity.livello[PlayerID].Exp >=
        Entity.livello[PlayerID].MaxExp
    )
    {
        Entity.livello[PlayerID].level++;

        Entity.livello[PlayerID].Exp -=
            Entity.livello[PlayerID].MaxExp;

        Entity.attacco[PlayerID].Damage++;

        Entity.salute[PlayerID].MaxHP++;

        Entity.salute[PlayerID].HP =
            Entity.salute[PlayerID].MaxHP;

        Entity.livello[PlayerID].PointStats += 3;

        double Scala =
            (double)rand() /
            (double)RAND_MAX;

        double Moltiplicatore =
            1.8 +
            Scala * 0.4;

        Entity.livello[PlayerID].MaxExp =
            (int)(
                Entity.livello[PlayerID].MaxExp *
                Moltiplicatore
            );

        if(Entity.livello[PlayerID].MaxExp < 1)
        {
            Entity.livello[PlayerID].MaxExp = 1;
        }

        system("cls");

        HANDLE hConsole =
            GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(
            hConsole,
            10
        );

        MoveCursor(47, 11);
        std::cout << "LEVEL UP!";

        SetConsoleTextAttribute(
            hConsole,
            15
        );

        MoveCursor(43, 14);
        std::cout
            << "LIVELLO: "
            << Entity.livello[PlayerID].level;

        MoveCursor(43, 16);
        std::cout
            << "HP: "
            << (int)Entity.salute[PlayerID].HP
            << "/"
            << (int)Entity.salute[PlayerID].MaxHP;

        MoveCursor(43, 18);
        std::cout
            << "ATK: "
            << (int)Entity.attacco[PlayerID].Damage;

        MoveCursor(43, 20);
        std::cout
            << "PUNTI STAT: +3";

        SetConsoleTextAttribute(
            hConsole,
            7
        );

        Sleep(1500);
    }
}

void CombatSystem(
    Map& m,
    EntityManager& Entity,
    EntityID PlayerID,
    EntityID EnemyID,
    EntityID BossID,
    bool& clean,
    int& GameState,
    int& SlimeKills,
    bool& BossSpawned
)
{
    static int BarTime = 0;
    static int EnemyBarTime = 0;
    static bool inSceltaMosse = false;
    static std::string BattleMessage;

    if(Entity.salute[PlayerID].MaxHP < 1)
    {
        Entity.salute[PlayerID].MaxHP = 10;
    }

    if(Entity.salute[PlayerID].HP > Entity.salute[PlayerID].MaxHP)
    {
        Entity.salute[PlayerID].MaxHP =
            Entity.salute[PlayerID].HP;
    }

    if(Entity.salute[PlayerID].HP < 0)
    {
        Entity.salute[PlayerID].HP = 0;
    }

    if(Entity.attacco[PlayerID].Damage < 1)
    {
        Entity.attacco[PlayerID].Damage = 2;
    }

    if(!clean)
    {
        system("cls");

        clean = true;
        BarTime = 0;
        EnemyBarTime = 0;
        inSceltaMosse = false;

        if(EnemyID == BossID)
        {
            BattleMessage =
                "L'ORCO ti ha trovato. Preparati!";
        }
        else
        {
            BattleMessage =
                "Il combattimento e' iniziato!";
        }

        DrawCombatFrame();
    }

    // =====================================================
    // =====================================================

    if(!inSceltaMosse)
    {
        if(BarTime < 100)
        {
            BarTime +=
                Entity.atb[PlayerID].Time;

            if(BarTime > 100)
            {
                BarTime = 100;
            }
        }

        if(EnemyBarTime < 100)
        {
            EnemyBarTime +=
                Entity.atb[EnemyID].Time;

            if(EnemyBarTime > 100)
            {
                EnemyBarTime = 100;
            }
        }
    }

    // =====================================================
    // =====================================================

    if(
        !inSceltaMosse &&
        EnemyBarTime >= 100
    )
    {
        int DannoNemico =
            (int)Entity.attacco[EnemyID].Damage;

        Entity.salute[PlayerID].HP -=
            DannoNemico;

        if(EnemyID == BossID)
        {
            BattleMessage =
                "L'ORCO ti ha colpito!";
        }
        else
        {
            BattleMessage =
                "Lo Slime ti ha colpito!";
        }

        EnemyBarTime = 0;
    }

    if(
        BarTime >= 100 &&
        !inSceltaMosse
    )
    {
        BattleMessage =
            "La tua barra e' piena! Premi [Q].";
    }

    DrawCombatGUI(
        Entity,
        PlayerID,
        EnemyID,
        BarTime,
        EnemyBarTime,
        BattleMessage
    );

    if(inSceltaMosse)
    {
        DrawCombatMoves(
            Entity,
            PlayerID
        );
    }
    else
    {
        HANDLE hConsole =
            GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(
            hConsole,
            8
        );

        MoveCursor(14, 28);

        std::cout
            << "[Q] ATTACCA                                                        ";

        SetConsoleTextAttribute(
            hConsole,
            7
        );
    }

    // =====================================================
    // =====================================================

    if(_kbhit())
    {
        char Combat =
            _getch();

        if(Combat == 'Q')
        {
            Combat = 'q';
        }

        if(
            !inSceltaMosse &&
            Combat == 'q' &&
            BarTime >= 100
        )
        {
            inSceltaMosse = true;

            BattleMessage =
                "Scegli quale attacco utilizzare.";
        }

        else if(
            inSceltaMosse &&
            Combat >= '1' &&
            Combat <= '9'
        )
        {
            int IndiceMossa =
                Combat - '1';

            const std::vector<Skill>& ListaMosse =
                Entity.skillset[PlayerID].skillset;

            if(
                IndiceMossa >= 0 &&
                IndiceMossa < (int)ListaMosse.size()
            )
            {
                Skill MossaScelta =
                    ListaMosse[IndiceMossa];

                int DannoTotale =
                    MossaScelta.Danno +
                    (int)Entity.attacco[PlayerID].Damage;

                Entity.salute[EnemyID].HP -=
                    DannoTotale;

                BattleMessage =
                    "Hai usato " +
                    MossaScelta.Name +
                    "!";

                // =========================================
                // =========================================

                if(Entity.salute[EnemyID].HP <= 0)
                {
                    Entity.salute[EnemyID].HP = 0;

                    DrawCombatGUI(
                        Entity,
                        PlayerID,
                        EnemyID,
                        BarTime,
                        EnemyBarTime,
                        BattleMessage
                    );

                    Sleep(500);

                    Entity.livello[PlayerID].Exp +=
                        Entity.livello[EnemyID].Exp;

                    LevelUp(
                        Entity,
                        PlayerID
                    );

                    // =====================================
                    // ORCO BOSS
                    // =====================================

                    if(EnemyID == BossID)
                    {
                        Entity.posizione.erase(
                            EnemyID
                        );

                        system("cls");

                        HANDLE hConsole =
                            GetStdHandle(STD_OUTPUT_HANDLE);

                        SetConsoleTextAttribute(
                            hConsole,
                            14
                        );

                        MoveCursor(48, 11);
                        std::cout << "ORCO SCONFITTO!";

                        MoveCursor(43, 13);
                        std::cout << "HAI SCONFITTO IL BOSS";

                        SetConsoleTextAttribute(
                            hConsole,
                            10
                        );

                        MoveCursor(50, 16);
                        std::cout
                            << "+"
                            << Entity.livello[EnemyID].Exp
                            << " EXP";

                        SetConsoleTextAttribute(
                            hConsole,
                            7
                        );

                        Sleep(2000);

                        clean = false;
                        GameState = 0;
                        system("cls");
                        return;
                    }

                    // =====================================
                    // =====================================

                    SlimeKills++;

                    Entity.posizione.erase(
                        EnemyID
                    );

                    Entity.RespawnTImer[EnemyID].timer =
                        250;

                    system("cls");

                    HANDLE hConsole =
                        GetStdHandle(STD_OUTPUT_HANDLE);

                    SetConsoleTextAttribute(
                        hConsole,
                        10
                    );

                    MoveCursor(48, 10);
                    std::cout << "SLIME SCONFITTO!";

                    MoveCursor(47, 12);
                    std::cout
                        << "SLIME UCCISI: "
                        << SlimeKills
                        << "/3";

                    MoveCursor(49, 14);
                    std::cout
                        << "+"
                        << Entity.livello[EnemyID].Exp
                        << " EXP";

                    Sleep(1100);

                    // =====================================
                    // =====================================

                    if(
                        SlimeKills >= 3 &&
                        !BossSpawned
                    )
                    {
                        BossSpawned = true;

                        Entity.salute[BossID].HP =
                            Entity.salute[BossID].MaxHP;

                        SpawnNemicoCasuale(
                            m,
                            Entity,
                            PlayerID,
                            BossID
                        );

                        system("cls");

                        SetConsoleTextAttribute(
                            hConsole,
                            12
                        );

                        MoveCursor(42, 10);
                        std::cout
                            << "QUALCOSA SI E' RISVEGLIATO...";

                        MoveCursor(48, 13);
                        std::cout
                            << "L'ORCO E' APPARSO";

                        SetConsoleTextAttribute(
                            hConsole,
                            7
                        );

                        Sleep(2200);
                    }

                    clean = false;
                    GameState = 0;
                    system("cls");
                    return;
                }

                BarTime = 0;
                inSceltaMosse = false;
            }
        }
    }

    // =====================================================
    // =====================================================

    if(Entity.salute[PlayerID].HP <= 0)
    {
        Entity.salute[PlayerID].HP = 0;

        system("cls");

        HANDLE hConsole =
            GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(
            hConsole,
            12
        );

        MoveCursor(49, 13);
        std::cout << "SEI STATO SCONFITTO";

        SetConsoleTextAttribute(
            hConsole,
            7
        );

        Sleep(1500);

        GameState = 2;
        return;
    }

    Sleep(100);
}


// =====================================================
// =====================================================

#pragma endregion

#pragma region RENDER_E_MOVIMENTO_MOSTRI

void AggiornaPlayer(
    Map& m,
    EntityManager& Entity,
    EntityID& ID,
    int VecchiaX,
    int VecchiaY
)
{
    const int MAP_WIDTH = 160;
    const int MAP_HEIGHT = 80;

    const int VIEW_WIDTH = 40;
    const int VIEW_HEIGHT = 24;

    const int SCREEN_X = 50;
    const int SCREEN_Y = 5;

    int PlayerX = Entity.posizione[ID].X;
    int PlayerY = Entity.posizione[ID].Y;

    int StartX = PlayerX - (VIEW_WIDTH / 2);
    int StartY = PlayerY - (VIEW_HEIGHT / 2);

    if(StartX < 0)
        StartX = 0;

    if(StartX > MAP_WIDTH - VIEW_WIDTH)
        StartX = MAP_WIDTH - VIEW_WIDTH;

    if(StartY < 0)
        StartY = 0;

    if(StartY > MAP_HEIGHT - VIEW_HEIGHT)
        StartY = MAP_HEIGHT - VIEW_HEIGHT;

    int OldStartX = VecchiaX - (VIEW_WIDTH / 2);
    int OldStartY = VecchiaY - (VIEW_HEIGHT / 2);

    if(OldStartX < 0)
        OldStartX = 0;

    if(OldStartX > MAP_WIDTH - VIEW_WIDTH)
        OldStartX = MAP_WIDTH - VIEW_WIDTH;

    if(OldStartY < 0)
        OldStartY = 0;

    if(OldStartY > MAP_HEIGHT - VIEW_HEIGHT)
        OldStartY = MAP_HEIGHT - VIEW_HEIGHT;

    if(
        StartX != OldStartX ||
        StartY != OldStartY
    )
    {
        MapRedering(m, Entity, ID);
        return;
    }

    auto DisegnaCella =
    [&](int WorldX, int WorldY)
    {
        int ScreenX =
            SCREEN_X + 1 + (WorldX - StartX);

        int ScreenY =
            SCREEN_Y + 1 + (WorldY - StartY);

        if(WorldX < StartX ||
           WorldX >= StartX + VIEW_WIDTH ||
           WorldY < StartY ||
           WorldY >= StartY + VIEW_HEIGHT)
        {
            return;
        }

        MoveCursor(ScreenX, ScreenY);

        bool EntityFound = false;

        for(
            std::map<EntityID, Posizione>::iterator IT =
            Entity.posizione.begin();

            IT != Entity.posizione.end();

            ++IT
        )
        {
            if(
                IT->second.X == WorldX &&
                IT->second.Y == WorldY
            )
            {
                if(IT->first == ID)
                {
                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_GREEN |
                        FOREGROUND_INTENSITY
                    );

                    StampaEntitaUTF8(Entity, ID, ID);
                }
                else
                {
                    SetColor(
                        FOREGROUND_RED |
                        FOREGROUND_INTENSITY
                    );

                    StampaEntitaUTF8(Entity, IT->first, ID);
                }

                EntityFound = true;
                break;
            }
        }

        if(EntityFound)
            return;

        int Cella = GetCellaMondo(m, WorldY, WorldX);

        switch(Cella)
        {
            case 0:
                SetColor(
                    FOREGROUND_GREEN |
                    FOREGROUND_BLUE
                );
                std::cout << " ";
                break;

            case 1:
                SetColor(
                    FOREGROUND_RED |
                    FOREGROUND_GREEN |
                    FOREGROUND_BLUE |
                    FOREGROUND_INTENSITY
                );
                std::cout << "\xE2\x96\x88";
                break;

            case 2:
                SetColor(
                    FOREGROUND_RED |
                    FOREGROUND_GREEN |
                    FOREGROUND_INTENSITY
                );
                std::cout << "\xE2\x96\x92";
                break;

            case 3:
                SetColor(
                    FOREGROUND_BLUE |
                    FOREGROUND_INTENSITY
                );
                std::cout << "\xE2\x96\x91";
                break;

            case 4:
                SetColor(
                    FOREGROUND_RED |
                    FOREGROUND_GREEN |
                    FOREGROUND_BLUE |
                    FOREGROUND_INTENSITY
                );
                std::cout << "\xE2\x97\x86";
                break;

            case 5:
                SetColor(
                    FOREGROUND_GREEN |
                    FOREGROUND_INTENSITY
                );
                std::cout << "\xE2\x99\xA3";
                break;

            case 6:
                SetColor(
                    FOREGROUND_RED |
                    FOREGROUND_GREEN |
                    FOREGROUND_INTENSITY
                );
                std::cout << "\xE2\x96\x88";
                break;

            case 7:
                SetColor(
                    FOREGROUND_RED |
                    FOREGROUND_INTENSITY
                );
                std::cout << "\xE2\x96\x93";
                break;

            case 8:
                SetColor(
                    FOREGROUND_RED |
                    FOREGROUND_GREEN
                );
                std::cout << "\xE2\x96\x88";
                break;


            case 24:
                SetColor(8);
                std::cout << "\xE2\x96\x93"; // ▓ recinto
                break;

            case 25:
                SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                std::cout << "\xE2\x80\xA0"; // † tomba
                break;

            case 26:
                SetColor(8);
                std::cout << "\xE2\x96\x91"; // ░ sentiero
                break;

            case 27:
                SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                std::cout << "\xE2\x96\x88"; // █ cripta
                break;

            case 28:
                SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
                std::cout << "\xE2\x96\x92"; // ▒ porta
                break;

            default:
                std::cout << " ";
                break;
        }
    };

    DisegnaCella(VecchiaX, VecchiaY);
    DisegnaCella(PlayerX, PlayerY);

    MoveCursor(
        SCREEN_X,
        SCREEN_Y + VIEW_HEIGHT + 3
    );

    SetColor(
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        FOREGROUND_BLUE |
        FOREGROUND_INTENSITY
    );

    std::cout
        << "HP: "
        << Entity.salute[ID].HP
        << "    ";

    std::cout
        << "ATK: "
        << Entity.attacco[ID].Damage
        << "    ";

    std::cout
        << "POS: "
        << PlayerX
        << ","
        << PlayerY
        << "        ";


    DrawZonePanel(
        m,
        Entity,
        ID,
        false
    );
}


// =====================================================
// =====================================================
static void RidisegnaCellaViewport(
    Map& m,
    EntityManager& Entity,
    EntityID PlayerID,
    int WorldX,
    int WorldY
)
{
    const int VIEW_WIDTH = 40;
    const int VIEW_HEIGHT = 24;
    const int SCREEN_X = 50;
    const int SCREEN_Y = 5;

    int PlayerX = Entity.posizione[PlayerID].X;
    int PlayerY = Entity.posizione[PlayerID].Y;

    int StartX = PlayerX - (VIEW_WIDTH / 2);
    int StartY = PlayerY - (VIEW_HEIGHT / 2);

    if(StartX < 0) StartX = 0;
    if(StartX > 160 - VIEW_WIDTH) StartX = 160 - VIEW_WIDTH;
    if(StartY < 0) StartY = 0;
    if(StartY > 80 - VIEW_HEIGHT) StartY = 80 - VIEW_HEIGHT;

    if(WorldX < StartX || WorldX >= StartX + VIEW_WIDTH ||
       WorldY < StartY || WorldY >= StartY + VIEW_HEIGHT)
    {
        return;
    }

    MoveCursor(
        SCREEN_X + 1 + (WorldX - StartX),
        SCREEN_Y + 1 + (WorldY - StartY)
    );

    for(
        std::map<EntityID, Posizione>::iterator IT = Entity.posizione.begin();
        IT != Entity.posizione.end();
        ++IT
    )
    {
        if(IT->second.X == WorldX && IT->second.Y == WorldY)
        {
            if(IT->first == PlayerID)
            {
                SetColor(
                    FOREGROUND_RED |
                    FOREGROUND_GREEN |
                    FOREGROUND_INTENSITY
                );
                StampaEntitaUTF8(Entity, PlayerID, PlayerID);
            }
            else
            {
                SetColor(
                    FOREGROUND_RED |
                    FOREGROUND_INTENSITY
                );
                StampaEntitaUTF8(Entity, IT->first, PlayerID);
            }

            SetColor(7);
            return;
        }
    }

    int Cella = GetCellaMondo(m, WorldY, WorldX);

    switch(Cella)
    {
        case 0:
            SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << " ";
            break;

        case 1:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x88";
            break;

        case 2:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x92";
            break;

        case 3:
            SetColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x89\x88";
            break;

        case 4:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x97\x86";
            break;

        case 5:
            SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x99\xA3";
            break;

        case 6:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x88";
            break;

        case 7:
            SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x93";
            break;

        case 8:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
            std::cout << "\xE2\x96\x88";
            break;

        case 9:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x88";
            break;

        case 10:
            SetColor(FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x93";
            break;

        case 11:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x88";
            break;

        case 12:
            SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x91";
            break;

        case 13:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
            std::cout << "\xE2\x96\x88";
            break;

        case 14:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "\xE2\x96\x91";
            break;

        case 15:
            SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x97\x8B";
            break;

        case 16:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
            std::cout << "\xE2\x96\x93";
            break;

        case 17:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x9C\xBF";
            break;

        case 18:
            SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x91";
            break;

        case 19:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x88";
            break;

        case 20:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
            std::cout << "\xE2\x96\x92";
            break;

        case 21:
            SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x93";
            break;

        case 22:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x88";
            break;

        case 23:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "\xE2\x96\x93";
            break;

        case 24:
            SetColor(8);
            std::cout << "\xE2\x96\x93"; // ▓ recinto
            break;

        case 25:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "\xE2\x80\xA0"; // † tomba
            break;

        case 26:
            SetColor(8);
            std::cout << "\xE2\x96\x91"; // ░ sentiero
            break;

        case 27:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "\xE2\x96\x88"; // █ cripta
            break;

        case 28:
            SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
            std::cout << "\xE2\x96\x92"; // ▒ porta
            break;

        default:
            SetColor(7);
            std::cout << " ";
            break;
    }

    SetColor(7);
}

void MuoviMostri(
    Map& m,
    EntityManager& Entity,
    EntityID& PlayerID,
    EntityID BossID
)
{
    int PlayerX = Entity.posizione[PlayerID].X;
    int PlayerY = Entity.posizione[PlayerID].Y;

    for(
        std::map<EntityID, Posizione>::iterator IT =
            Entity.posizione.begin();
        IT != Entity.posizione.end();
        ++IT
    )
    {
        EntityID MobID = IT->first;

        if(MobID == PlayerID)
            continue;

        int MobX = IT->second.X;
        int MobY = IT->second.Y;

        int DX = PlayerX - MobX;
        int DY = PlayerY - MobY;

        if(MobID != BossID)
        {
            int RaggioX = 12;
            int RaggioY = 12;

            if(Entity.entityrange.count(MobID) > 0)
            {
                RaggioX =
                    Entity.entityrange[MobID].AttackRangeX;

                RaggioY =
                    Entity.entityrange[MobID].AttackRangeY;
            }

            if(
                abs(DX) > RaggioX ||
                abs(DY) > RaggioY
            )
            {
                continue;
            }
        }

        int NuovaX = MobX;
        int NuovaY = MobY;

        if(abs(DX) >= abs(DY))
        {
            if(DX > 0)
                NuovaX++;
            else if(DX < 0)
                NuovaX--;
        }
        else
        {
            if(DY > 0)
                NuovaY++;
            else if(DY < 0)
                NuovaY--;
        }

        bool PuoMuoversi = true;

        if(
            NuovaX < 0 ||
            NuovaX >= 160 ||
            NuovaY < 0 ||
            NuovaY >= 80
        )
        {
            PuoMuoversi = false;
        }
        else
        {
            int Cella =
                GetCellaMondo(
                    m,
                    NuovaY,
                    NuovaX
                );

            if(
                Cella == 1 ||
                Cella == 3 ||
                Cella == 4 ||
                Cella == 5 ||
                Cella == 6 ||
                Cella == 7 ||
                Cella == 8 ||
                Cella == 24 ||
                Cella == 25 ||
                Cella == 27
            )
            {
                PuoMuoversi = false;
            }
        }

        if(PuoMuoversi)
        {
            for(
                std::map<EntityID, Posizione>::iterator Altro =
                    Entity.posizione.begin();
                Altro != Entity.posizione.end();
                ++Altro
            )
            {
                if(
                    Altro->first == MobID ||
                    Altro->first == PlayerID
                )
                {
                    continue;
                }

                if(
                    Altro->second.X == NuovaX &&
                    Altro->second.Y == NuovaY
                )
                {
                    PuoMuoversi = false;
                    break;
                }
            }
        }

        if(!PuoMuoversi)
        {
            NuovaX = MobX;
            NuovaY = MobY;

            if(abs(DY) > 0)
            {
                if(DY > 0)
                    NuovaY++;
                else
                    NuovaY--;
            }
            else
            {
                if(DX > 0)
                    NuovaX++;
                else if(DX < 0)
                    NuovaX--;
            }

            PuoMuoversi = false;

            if(
                NuovaX >= 0 &&
                NuovaX < 160 &&
                NuovaY >= 0 &&
                NuovaY < 80
            )
            {
                int Cella =
                    GetCellaMondo(
                        m,
                        NuovaY,
                        NuovaX
                    );

                if(
                    Cella != 1 &&
                    Cella != 3 &&
                    Cella != 4 &&
                    Cella != 5 &&
                    Cella != 6 &&
                    Cella != 7 &&
                    Cella != 8 &&
                    Cella != 24 &&
                    Cella != 25 &&
                    Cella != 27
                )
                {
                    PuoMuoversi = true;
                }
            }

            if(PuoMuoversi)
            {
                for(
                    std::map<EntityID, Posizione>::iterator Altro =
                        Entity.posizione.begin();
                    Altro != Entity.posizione.end();
                    ++Altro
                )
                {
                    if(
                        Altro->first == MobID ||
                        Altro->first == PlayerID
                    )
                    {
                        continue;
                    }

                    if(
                        Altro->second.X == NuovaX &&
                        Altro->second.Y == NuovaY
                    )
                    {
                        PuoMuoversi = false;
                        break;
                    }
                }
            }
        }

        if(PuoMuoversi)
        {
            Entity.posizione[MobID].X = NuovaX;
            Entity.posizione[MobID].Y = NuovaY;
        }
    }
}

// =====================================================
// =====================================================

#pragma endregion

#pragma region GAME_OVER

static bool CaricaUltimoSalvataggio(
    SlotManager& Slots,
    EntityManager& Entity,
    EntityID PlayerID
)
{
    std::string NomeFile;

    if(Slots.GetChoice() == 1)
    {
        NomeFile = "Slot_1";
    }
    else if(Slots.GetChoice() == 2)
    {
        NomeFile = "Slot_2";
    }
    else if(Slots.GetChoice() == 3)
    {
        NomeFile = "Slot_3";
    }
    else
    {
        return false;
    }

    std::ifstream file(
        NomeFile.c_str()
    );

    if(!file.is_open())
    {
        return false;
    }

    char Simbolo = '@';
    int PosX = 100;
    int PosY = 50;
    double HP = 10;
    double MaxHP = 10;
    double Damage = 2;
    size_t NumeroSkill = 0;

    if(!(file >> Simbolo))
    {
        file.close();
        return false;
    }

    if(!(file >> PosX))
    {
        file.close();
        return false;
    }

    if(!(file >> PosY))
    {
        file.close();
        return false;
    }

    if(!(file >> HP))
    {
        file.close();
        return false;
    }

    if(!(file >> MaxHP))
    {
        file.close();
        return false;
    }

    if(!(file >> Damage))
    {
        file.close();
        return false;
    }

    if(!(file >> NumeroSkill))
    {
        file.close();
        return false;
    }

    if(
        PosX < 0 ||
        PosX >= 160 ||
        PosY < 0 ||
        PosY >= 80
    )
    {
        PosX = 100;
        PosY = 50;
    }

    if(MaxHP < 1)
    {
        MaxHP = 10;
    }

    if(HP < 0)
    {
        HP = 0;
    }

    if(HP > MaxHP)
    {
        if(HP >= 5 && MaxHP <= 3)
        {
            MaxHP = HP;
        }
        else
        {
            HP = MaxHP;
        }
    }

    if(Damage < 1)
    {
        Damage = 2;
    }

    if(NumeroSkill > 20)
    {
        file.close();
        return false;
    }

    std::vector<Skill> SkillCaricate;

    for(size_t i = 0; i < NumeroSkill; i++)
    {
        Skill SkillCaricata;

        if(!(file >> SkillCaricata.Danno))
        {
            file.close();
            return false;
        }

        if(!(file >> SkillCaricata.Name))
        {
            file.close();
            return false;
        }

        SkillCaricate.push_back(
            SkillCaricata
        );
    }

    int Exp = 0;
    int MaxExp = 10;
    int Livello = 0;
    int PointStats = 0;

    if(!(file >> Exp))
    {
        Exp = 0;
    }

    if(!(file >> MaxExp))
    {
        MaxExp = 10;
    }

    if(!(file >> Livello))
    {
        Livello = 0;
    }

    if(!(file >> PointStats))
    {
        PointStats = 0;
    }

    file.close();

    if(MaxExp < 1)
    {
        MaxExp = 10;
    }

    if(Exp < 0)
    {
        Exp = 0;
    }

    if(Livello < 0)
    {
        Livello = 0;
    }

    if(PointStats < 0)
    {
        PointStats = 0;
    }

    Entity.visuale[PlayerID].Simbolo = Simbolo;
    Entity.posizione[PlayerID].X = PosX;
    Entity.posizione[PlayerID].Y = PosY;
    Entity.salute[PlayerID].HP = HP;
    Entity.salute[PlayerID].MaxHP = MaxHP;
    Entity.attacco[PlayerID].Damage = Damage;

    Entity.skillset[PlayerID].skillset =
        SkillCaricate;

    Entity.skillset[PlayerID].skillImparate =
        (int)SkillCaricate.size();

    Entity.livello[PlayerID].Exp = Exp;
    Entity.livello[PlayerID].MaxExp = MaxExp;
    Entity.livello[PlayerID].level = Livello;
    Entity.livello[PlayerID].PointStats = PointStats;

    return true;
}

static void LiberaPosizionePlayerDopoCaricamento(
    Map& m,
    EntityManager& Entity,
    EntityID PlayerID
)
{
    if(
        Entity.posizione.count(PlayerID) == 0
    )
    {
        return;
    }

    int PlayerX =
        Entity.posizione[PlayerID].X;

    int PlayerY =
        Entity.posizione[PlayerID].Y;

    std::vector<EntityID> DaSpostare;

    for(
        std::map<EntityID, Posizione>::iterator IT =
            Entity.posizione.begin();
        IT != Entity.posizione.end();
        ++IT
    )
    {
        if(IT->first == PlayerID)
        {
            continue;
        }

        if(
            IT->second.X == PlayerX &&
            IT->second.Y == PlayerY
        )
        {
            DaSpostare.push_back(
                IT->first
            );
        }
    }

    for(size_t i = 0; i < DaSpostare.size(); i++)
    {
        EntityID EnemyID =
            DaSpostare[i];

        Entity.salute[EnemyID].HP =
            Entity.salute[EnemyID].MaxHP;

        SpawnNemicoCasuale(
            m,
            Entity,
            PlayerID,
            EnemyID
        );
    }
}


static void DrawGameOver()
{
    HANDLE hConsole =
        GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");

    SetConsoleTextAttribute(
        hConsole,
        FOREGROUND_RED |
        FOREGROUND_INTENSITY
    );

    MoveCursor(52, 10);
    std::cout << "GAME OVER";

    SetConsoleTextAttribute(
        hConsole,
        15
    );

    MoveCursor(45, 14);
    std::cout << "SEI STATO SCONFITTO";

    SetConsoleTextAttribute(
        hConsole,
        11
    );

    MoveCursor(39, 19);
    std::cout << "[R] RIPROVA DALL'ULTIMO SALVATAGGIO";

    MoveCursor(43, 22);
    std::cout << "[M] TORNA AL MENU PRINCIPALE";

    SetConsoleTextAttribute(
        hConsole,
        8
    );

    MoveCursor(46, 26);
    std::cout << "ESC = MENU PRINCIPALE";

    SetConsoleTextAttribute(
        hConsole,
        7
    );
}


#pragma endregion

#pragma region GAME_LOOP

void GameInExecution(
    Map& m,
    EntityManager& Entity,
    EntityID& ID,
    SlotManager& Slots,
    EntityID BossID
)
{
    SetupUnicodeConsole();
    HideCursor();

    system("mode con cols=130 lines=50");

    StartMenu Menu;
    StatsGUI Stats;

    bool InExecution = true;
    bool clean = false;

    int GameState = 0;

    EntityID CurrentEnemyID = -1;

    int SlimeKills = 0;
    bool BossSpawned = false;
    bool BossSconfitto = false;
    DWORD ProtezioneDopoRetryFino = 0;

    system("cls");

    MapRedering(
        m,
        Entity,
        ID
    );

    DrawProgressioneNemici(
        SlimeKills,
        BossSpawned,
        !BossSconfitto
    );

    while(InExecution)
    {
        // =================================================
        // MAPPA
        // =================================================

        if(GameState == 0)
        {
            GestisciRespawnNemici(
                m,
                Entity,
                ID,
                BossID
            );

            if(
                BossSpawned &&
                Entity.posizione.count(BossID) == 0 &&
                Entity.salute[BossID].HP <= 0
            )
            {
                BossSconfitto = true;
            }

            DrawProgressioneNemici(
                SlimeKills,
                BossSpawned,
                !BossSconfitto
            );

            if(_kbhit())
            {
                char Tasto = _getch();

                // =========================================
                // =========================================

                if(Tasto == 27)
                {
                    Slots.SaveData(
                        Entity,
                        ID
                    );

                    system("cls");

                    Menu.SetAttivo(true);

                    while(Menu.GetAttivo())
                    {
                        Menu.MenuGraphic();
                        Menu.Input();
                    }

                    system("cls");

                    MapRedering(
                        m,
                        Entity,
                        ID
                    );

                    DrawProgressioneNemici(
                        SlimeKills,
                        BossSpawned,
                        !BossSconfitto
                    );
                }

                // =========================================
                // M -> STATISTICHE
                // =========================================

                else if(
                    Tasto == 'm' ||
                    Tasto == 'M'
                )
                {
                    system("cls");

                    Stats.SetisOpen(true);

                    while(Stats.GetisOpen())
                    {
                        Stats.StatsGraphic(
                            Entity,
                            ID
                        );

                        Stats.Input(
                            Entity,
                            ID
                        );

                        Sleep(20);
                    }

                    Slots.SaveData(
                        Entity,
                        ID
                    );

                    system("cls");

                    MapRedering(
                        m,
                        Entity,
                        ID
                    );

                    DrawProgressioneNemici(
                        SlimeKills,
                        BossSpawned,
                        !BossSconfitto
                    );
                }

                // =========================================
                // P -> PERSONAGGIO
                // =========================================

                else if(
                    Tasto == 'p' ||
                    Tasto == 'P'
                )
                {
                    MostraPersonaggio(
                        Entity,
                        ID
                    );

                    system("cls");

                    MapRedering(
                        m,
                        Entity,
                        ID
                    );

                    DrawProgressioneNemici(
                        SlimeKills,
                        BossSpawned,
                        !BossSconfitto
                    );
                }

                // =========================================
                // MOVIMENTO
                // =========================================

                else
                {
                    if(Tasto == 'W') Tasto = 'w';
                    if(Tasto == 'A') Tasto = 'a';
                    if(Tasto == 'S') Tasto = 's';
                    if(Tasto == 'D') Tasto = 'd';

                    int VecchiaX =
                        Entity.posizione[ID].X;

                    int VecchiaY =
                        Entity.posizione[ID].Y;

                    MuoviEntita(
                        m,
                        Entity,
                        ID,
                        Tasto
                    );

                    int NuovaX =
                        Entity.posizione[ID].X;

                    int NuovaY =
                        Entity.posizione[ID].Y;

                    if(
                        VecchiaX != NuovaX ||
                        VecchiaY != NuovaY
                    )
                    {
                        AggiornaPlayer(
                            m,
                            Entity,
                            ID,
                            VecchiaX,
                            VecchiaY
                        );
                    }

                    EntityID hitEnemy =
                        CollisionDetection(
                            Entity,
                            ID
                        );

                    if(hitEnemy != -1)
                    {
                        GameState = 1;
                        CurrentEnemyID = hitEnemy;
                        clean = false;
                    }
                }
            }
        }

        // =================================================
        // COMBATTIMENTO
        // =================================================

        else if(GameState == 1)
        {
            CombatSystem(
                m,
                Entity,
                ID,
                CurrentEnemyID,
                BossID,
                clean,
                GameState,
                SlimeKills,
                BossSpawned
            );

            if(GameState == 0)
            {
                if(
                    BossSpawned &&
                    Entity.salute[BossID].HP <= 0
                )
                {
                    BossSconfitto = true;
                }

                MapRedering(
                    m,
                    Entity,
                    ID
                );

                DrawProgressioneNemici(
                    SlimeKills,
                    BossSpawned,
                    !BossSconfitto
                );
            }
        }

        // =================================================
        // GAME OVER
        // =================================================

        else if(GameState == 2)
        {
            DrawGameOver();

            bool SceltaGameOver = false;

            while(!SceltaGameOver)
            {
                if(_kbhit())
                {
                    char TastoGameOver = _getch();

                    if(
                        TastoGameOver == 'r' ||
                        TastoGameOver == 'R'
                    )
                    {
                        bool Caricato =
                            CaricaUltimoSalvataggio(
                                Slots,
                                Entity,
                                ID
                            );

                        if(!Caricato)
                        {
                            if(Entity.salute[ID].MaxHP < 1)
                            {
                                Entity.salute[ID].MaxHP = 10;
                            }

                            if(Entity.attacco[ID].Damage < 1)
                            {
                                Entity.attacco[ID].Damage = 2;
                            }

                            Entity.salute[ID].HP =
                                Entity.salute[ID].MaxHP;
                        }
                        else
                        {
                            if(Entity.salute[ID].MaxHP < 1)
                            {
                                Entity.salute[ID].MaxHP = 10;
                            }

                            if(Entity.attacco[ID].Damage < 1)
                            {
                                Entity.attacco[ID].Damage = 2;
                            }

                            Entity.salute[ID].HP =
                                Entity.salute[ID].MaxHP;

                            LiberaPosizionePlayerDopoCaricamento(
                                m,
                                Entity,
                                ID
                            );

                            Slots.SaveData(
                                Entity,
                                ID
                            );
                        }

                        ProtezioneDopoRetryFino =
                            GetTickCount() + 2000;

                        GameState = 0;
                        clean = false;
                        CurrentEnemyID = -1;

                        system("cls");

                        MapRedering(
                            m,
                            Entity,
                            ID
                        );

                        DrawProgressioneNemici(
                            SlimeKills,
                            BossSpawned,
                            !BossSconfitto
                        );

                        SceltaGameOver = true;
                    }
                    else if(
                        TastoGameOver == 'm' ||
                        TastoGameOver == 'M' ||
                        TastoGameOver == 27
                    )
                    {
                        system("cls");

                        InExecution = false;
                        SceltaGameOver = true;
                    }
                }

                Sleep(20);
            }

            if(!InExecution)
            {
                return;
            }
        }

        // =================================================
        // MOVIMENTO MOSTRI
        // =================================================

        static DWORD UltimoMovimentoMostri = 0;

        DWORD Ora =
            GetTickCount();

        if(
            GameState == 0 &&
            Ora >= ProtezioneDopoRetryFino &&
            Ora - UltimoMovimentoMostri >= 500
        )
        {
            std::map<EntityID, Posizione> VecchiePosizioni =
                Entity.posizione;

            MuoviMostri(
                m,
                Entity,
                ID,
                BossID
            );

            UltimoMovimentoMostri =
                Ora;

            for(
                std::map<EntityID, Posizione>::iterator IT =
                    Entity.posizione.begin();
                IT != Entity.posizione.end();
                ++IT
            )
            {
                EntityID MobID = IT->first;

                if(MobID == ID)
                    continue;

                std::map<EntityID, Posizione>::iterator Vecchia =
                    VecchiePosizioni.find(MobID);

                if(Vecchia == VecchiePosizioni.end())
                {
                    RidisegnaCellaViewport(
                        m, Entity, ID,
                        IT->second.X,
                        IT->second.Y
                    );
                    continue;
                }

                if(
                    Vecchia->second.X != IT->second.X ||
                    Vecchia->second.Y != IT->second.Y
                )
                {
                    RidisegnaCellaViewport(
                        m, Entity, ID,
                        Vecchia->second.X,
                        Vecchia->second.Y
                    );

                    RidisegnaCellaViewport(
                        m, Entity, ID,
                        IT->second.X,
                        IT->second.Y
                    );
                }
            }

            DrawZonePanel(
                m,
                Entity,
                ID,
                false
            );

            DrawProgressioneNemici(
                SlimeKills,
                BossSpawned,
                !BossSconfitto
            );

            EntityID hitEnemy =
                CollisionDetection(
                    Entity,
                    ID
                );

            if(hitEnemy != -1)
            {
                GameState = 1;
                CurrentEnemyID = hitEnemy;
                clean = false;
            }
        }

        Sleep(20);
    }
}

#pragma endregion
