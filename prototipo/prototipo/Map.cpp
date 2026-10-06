#include "stdafx.h"
#include "Map.h"

Map::Map()
{
    // =====================================================
    // TERRENO BASE
    // =====================================================

    for(int y = 0; y < 80; y++)
    {
        for(int x = 0; x < 160; x++)
        {
            Griglia[y][x] = 0;
        }
    }


    // =====================================================
    // FORESTA ESTERNA
    // =====================================================

    for(int x = 0; x < 160; x++)
    {
        Griglia[0][x] = 5;
        Griglia[1][x] = 5;
        Griglia[78][x] = 5;
        Griglia[79][x] = 5;
    }

    for(int y = 0; y < 80; y++)
    {
        Griglia[y][0] = 5;
        Griglia[y][1] = 5;
        Griglia[y][158] = 5;
        Griglia[y][159] = 5;
    }


    // =====================================================
    // FIUME
    // =====================================================

    // TRATTO NORD
    for(int y = 2; y < 17; y++)
    {
        Griglia[y][105] = 3;
        Griglia[y][106] = 3;
        Griglia[y][107] = 3;
        Griglia[y][108] = 3;
    }

    // raccordo
    Griglia[16][104] = 3;
    Griglia[16][105] = 3;
    Griglia[16][106] = 3;
    Griglia[16][107] = 3;
    Griglia[16][108] = 3;

    Griglia[17][103] = 3;
    Griglia[17][104] = 3;
    Griglia[17][105] = 3;
    Griglia[17][106] = 3;
    Griglia[17][107] = 3;


    // SECONDO TRATTO
    for(int y = 17; y < 27; y++)
    {
        Griglia[y][102] = 3;
        Griglia[y][103] = 3;
        Griglia[y][104] = 3;
        Griglia[y][105] = 3;
    }

    Griglia[26][101] = 3;
    Griglia[26][102] = 3;
    Griglia[26][103] = 3;
    Griglia[26][104] = 3;
    Griglia[26][105] = 3;

    Griglia[27][99] = 3;
    Griglia[27][100] = 3;
    Griglia[27][101] = 3;
    Griglia[27][102] = 3;
    Griglia[27][103] = 3;


    // TRATTO CENTRALE
    for(int y = 27; y < 48; y++)
    {
        Griglia[y][98] = 3;
        Griglia[y][99] = 3;
        Griglia[y][100] = 3;
        Griglia[y][101] = 3;
    }

    Griglia[47][98] = 3;
    Griglia[47][99] = 3;
    Griglia[47][100] = 3;
    Griglia[47][101] = 3;
    Griglia[47][102] = 3;

    Griglia[48][99] = 3;
    Griglia[48][100] = 3;
    Griglia[48][101] = 3;
    Griglia[48][102] = 3;
    Griglia[48][103] = 3;


    // TRATTO CENTRO-SUD
    for(int y = 48; y < 58; y++)
    {
        Griglia[y][100] = 3;
        Griglia[y][101] = 3;
        Griglia[y][102] = 3;
        Griglia[y][103] = 3;
    }

    Griglia[57][100] = 3;
    Griglia[57][101] = 3;
    Griglia[57][102] = 3;
    Griglia[57][103] = 3;
    Griglia[57][104] = 3;

    Griglia[58][101] = 3;
    Griglia[58][102] = 3;
    Griglia[58][103] = 3;
    Griglia[58][104] = 3;
    Griglia[58][105] = 3;

    Griglia[59][102] = 3;
    Griglia[59][103] = 3;
    Griglia[59][104] = 3;
    Griglia[59][105] = 3;
    Griglia[59][106] = 3;


    // TRATTO SUD
    for(int y = 58; y < 80; y++)
    {
        Griglia[y][104] = 3;
        Griglia[y][105] = 3;
        Griglia[y][106] = 3;
        Griglia[y][107] = 3;
    }


    // =====================================================
    // LAGHETTO DETTAGLIATO
    // =====================================================

    for(int y = 54; y <= 69; y++)
    {
        for(int x = 117; x <= 145; x++)
        {
            bool acqua = false;

            if(y == 54 && x >= 127 && x <= 136)
                acqua = true;

            if(y == 55 && x >= 124 && x <= 139)
                acqua = true;

            if(y == 56 && x >= 122 && x <= 141)
                acqua = true;

            if(y == 57 && x >= 121 && x <= 142)
                acqua = true;

            if(y == 58 && x >= 120 && x <= 143)
                acqua = true;

            if(y == 59 && x >= 119 && x <= 143)
                acqua = true;

            if(y == 60 && x >= 119 && x <= 144)
                acqua = true;

            if(y == 61 && x >= 118 && x <= 144)
                acqua = true;

            if(y == 62 && x >= 118 && x <= 144)
                acqua = true;

            if(y == 63 && x >= 119 && x <= 144)
                acqua = true;

            if(y == 64 && x >= 119 && x <= 143)
                acqua = true;

            if(y == 65 && x >= 120 && x <= 143)
                acqua = true;

            if(y == 66 && x >= 121 && x <= 142)
                acqua = true;

            if(y == 67 && x >= 122 && x <= 141)
                acqua = true;

            if(y == 68 && x >= 124 && x <= 139)
                acqua = true;

            if(y == 69 && x >= 127 && x <= 136)
                acqua = true;

            if(acqua)
            {
                Griglia[y][x] = 3;
            }
        }
    }


    // =====================================================
    // SENTIERO PRINCIPALE - OVEST
    // =====================================================

    for(int x = 4; x <= 17; x++)
    {
        Griglia[40][x] = 2;
        Griglia[41][x] = 2;
    }

    for(int x = 18; x <= 25; x++)
    {
        Griglia[39][x] = 2;
        Griglia[40][x] = 2;
    }

    Griglia[39][17] = 2;
    Griglia[40][17] = 2;
    Griglia[40][18] = 2;
    Griglia[41][18] = 2;

    for(int x = 26; x <= 32; x++)
    {
        Griglia[38][x] = 2;
        Griglia[39][x] = 2;
    }

    Griglia[38][32] = 2;
    Griglia[39][32] = 2;
    Griglia[39][33] = 2;
    Griglia[40][33] = 2;


    // =====================================================
    // SENTIERO NORD
    // =====================================================

    for(int y = 7; y <= 11; y++)
    {
        Griglia[y][23] = 2;
        Griglia[y][24] = 2;
    }

    for(int y = 12; y <= 16; y++)
    {
        Griglia[y][24] = 2;
        Griglia[y][25] = 2;
    }

    Griglia[11][23] = 2;
    Griglia[11][24] = 2;
    Griglia[11][25] = 2;

    for(int y = 17; y <= 22; y++)
    {
        Griglia[y][23] = 2;
        Griglia[y][24] = 2;
    }

    Griglia[16][23] = 2;
    Griglia[16][24] = 2;
    Griglia[16][25] = 2;

    for(int y = 23; y <= 28; y++)
    {
        Griglia[y][22] = 2;
        Griglia[y][23] = 2;
    }

    Griglia[22][22] = 2;
    Griglia[22][23] = 2;
    Griglia[22][24] = 2;

    for(int y = 29; y <= 34; y++)
    {
        Griglia[y][23] = 2;
        Griglia[y][24] = 2;
    }

    Griglia[28][22] = 2;
    Griglia[28][23] = 2;
    Griglia[28][24] = 2;

    for(int y = 35; y <= 39; y++)
    {
        Griglia[y][24] = 2;
        Griglia[y][25] = 2;
    }

    Griglia[34][23] = 2;
    Griglia[34][24] = 2;
    Griglia[34][25] = 2;

    for(int x = 24; x <= 32; x++)
    {
        Griglia[38][x] = 2;
        Griglia[39][x] = 2;
    }


    // =====================================================
    // VILLAGGIO - PIAZZA IN PIETRA
    // =====================================================

    for(int y = 31; y <= 50; y++)
    {
        for(int x = 36; x <= 72; x++)
        {
            Griglia[y][x] = 14;
        }
    }


    // =====================================================
    // STRADE INTERNE DEL VILLAGGIO
    // =====================================================

    for(int x = 33; x <= 75; x++)
    {
        Griglia[39][x] = 2;
        Griglia[40][x] = 2;
    }

    for(int y = 31; y <= 55; y++)
    {
        Griglia[y][53] = 2;
        Griglia[y][54] = 2;
    }


    // =====================================================
    // CASA 1
    // =====================================================

    for(int y = 27; y <= 28; y++)
    {
        for(int x = 38; x <= 47; x++)
        {
            Griglia[y][x] = 7;
        }
    }

    for(int y = 29; y <= 33; y++)
    {
        for(int x = 39; x <= 46; x++)
        {
            Griglia[y][x] = 6;
        }
    }

    Griglia[31][39] = 18;
    Griglia[31][46] = 18;

    Griglia[33][42] = 8;

    Griglia[27][45] = 23;


    // =====================================================
    // CASA 2
    // =====================================================

    for(int y = 26; y <= 27; y++)
    {
        for(int x = 51; x <= 61; x++)
        {
            Griglia[y][x] = 7;
        }
    }

    for(int y = 28; y <= 33; y++)
    {
        for(int x = 52; x <= 60; x++)
        {
            Griglia[y][x] = 6;
        }
    }

    Griglia[30][52] = 18;
    Griglia[30][60] = 18;

    Griglia[33][56] = 8;

    Griglia[26][59] = 23;


    // =====================================================
    // CASA 3
    // =====================================================

    for(int y = 29; y <= 30; y++)
    {
        for(int x = 64; x <= 72; x++)
        {
            Griglia[y][x] = 7;
        }
    }

    for(int y = 31; y <= 35; y++)
    {
        for(int x = 65; x <= 71; x++)
        {
            Griglia[y][x] = 6;
        }
    }

    Griglia[33][65] = 18;
    Griglia[33][71] = 18;

    Griglia[35][68] = 8;

    Griglia[29][70] = 23;


    // =====================================================
    // CASA DEL PNG / LORE
    // =====================================================

    for(int y = 43; y <= 44; y++)
    {
        for(int x = 47; x <= 58; x++)
        {
            Griglia[y][x] = 7;
        }
    }

    for(int y = 45; y <= 50; y++)
    {
        for(int x = 48; x <= 57; x++)
        {
            Griglia[y][x] = 6;
        }
    }

    Griglia[47][48] = 18;
    Griglia[47][57] = 18;

    Griglia[50][52] = 8;

    Griglia[43][55] = 23;


    // =====================================================
    // FONTANA / POZZO
    // =====================================================

    Griglia[36][58] = 15;
    Griglia[36][59] = 15;
    Griglia[36][60] = 15;
    Griglia[36][61] = 15;
    Griglia[36][62] = 15;

    Griglia[37][58] = 15;
    Griglia[37][62] = 15;

    Griglia[38][58] = 15;
    Griglia[38][62] = 15;

    Griglia[37][59] = 3;
    Griglia[37][60] = 3;
    Griglia[37][61] = 3;

    Griglia[38][59] = 3;
    Griglia[38][60] = 3;
    Griglia[38][61] = 3;

    Griglia[39][58] = 15;
    Griglia[39][59] = 15;
    Griglia[39][60] = 15;
    Griglia[39][61] = 15;
    Griglia[39][62] = 15;


    // =====================================================
    // PANCHINE VILLAGGIO
    // =====================================================

    Griglia[35][55] = 20;
    Griglia[35][56] = 20;

    Griglia[41][59] = 20;
    Griglia[41][60] = 20;


    // =====================================================
    // MERCATO
    // =====================================================

    Griglia[36][38] = 21;
    Griglia[36][39] = 21;
    Griglia[36][40] = 21;

    Griglia[37][38] = 16;
    Griglia[37][40] = 16;

    Griglia[44][66] = 21;
    Griglia[44][67] = 21;
    Griglia[44][68] = 21;

    Griglia[45][66] = 16;
    Griglia[45][68] = 16;


    // =====================================================
    // CASSE E BARILI
    // =====================================================

    Griglia[34][44] = 16;
    Griglia[34][45] = 16;

    Griglia[34][59] = 16;

    Griglia[36][71] = 16;

    Griglia[48][60] = 16;
    Griglia[49][60] = 16;


    // =====================================================
    // LAMPIONI
    // =====================================================

    Griglia[35][35] = 19;
    Griglia[35][74] = 19;

    Griglia[42][37] = 19;
    Griglia[42][71] = 19;

    Griglia[52][42] = 19;
    Griglia[52][65] = 19;


    // =====================================================
    // FIORI
    // =====================================================

    Griglia[32][36] = 17;
    Griglia[33][36] = 17;
    Griglia[34][36] = 17;

    Griglia[46][39] = 17;
    Griglia[47][39] = 17;
    Griglia[48][39] = 17;

    Griglia[46][70] = 17;
    Griglia[47][70] = 17;
    Griglia[48][70] = 17;

    Griglia[52][46] = 17;
    Griglia[52][47] = 17;

    Griglia[52][60] = 17;
    Griglia[52][61] = 17;


    // =====================================================
    // CARTELLI
    // =====================================================

    Griglia[38][35] = 22;
    Griglia[41][73] = 22;
    Griglia[53][55] = 22;


    // =====================================================
    // MURETTO DEL VILLAGGIO
    // =====================================================

    for(int x = 33; x <= 75; x++)
    {
        Griglia[24][x] = 1;
        Griglia[55][x] = 1;
    }

    for(int y = 24; y <= 55; y++)
    {
        Griglia[y][33] = 1;
        Griglia[y][75] = 1;
    }


    // ingressi
    Griglia[39][33] = 2;
    Griglia[40][33] = 2;

    Griglia[39][75] = 2;
    Griglia[40][75] = 2;

    Griglia[55][53] = 2;
    Griglia[55][54] = 2;


    // =====================================================
    // USCITA EST DEL VILLAGGIO
    // =====================================================

    for(int x = 76; x <= 84; x++)
    {
        Griglia[39][x] = 2;
        Griglia[40][x] = 2;
    }

    for(int x = 85; x <= 91; x++)
    {
        Griglia[38][x] = 2;
        Griglia[39][x] = 2;
    }

    Griglia[38][84] = 2;
    Griglia[39][84] = 2;
    Griglia[39][85] = 2;
    Griglia[40][85] = 2;

    for(int x = 92; x <= 97; x++)
    {
        Griglia[39][x] = 2;
        Griglia[40][x] = 2;
    }


    // =====================================================
    // PONTE CENTRALE
    // =====================================================

    for(int x = 95; x <= 103; x++)
    {
        Griglia[39][x] = 2;
        Griglia[40][x] = 2;
    }


    // =====================================================
    // SENTIERO DOPO IL FIUME
    // =====================================================

    for(int x = 104; x <= 113; x++)
    {
        Griglia[39][x] = 2;
        Griglia[40][x] = 2;
    }

    for(int x = 114; x <= 122; x++)
    {
        Griglia[40][x] = 2;
        Griglia[41][x] = 2;
    }

    Griglia[39][113] = 2;
    Griglia[40][113] = 2;
    Griglia[40][114] = 2;
    Griglia[41][114] = 2;

    for(int x = 123; x <= 132; x++)
    {
        Griglia[41][x] = 2;
        Griglia[42][x] = 2;
    }

    for(int x = 133; x <= 143; x++)
    {
        Griglia[40][x] = 2;
        Griglia[41][x] = 2;
    }

    Griglia[40][132] = 2;
    Griglia[41][132] = 2;
    Griglia[41][133] = 2;
    Griglia[42][133] = 2;

    for(int x = 144; x <= 155; x++)
    {
        Griglia[39][x] = 2;
        Griglia[40][x] = 2;
    }

    Griglia[39][143] = 2;
    Griglia[40][143] = 2;
    Griglia[40][144] = 2;
    Griglia[41][144] = 2;


    // =====================================================
    // SENTIERO VERSO IL LAGHETTO
    // =====================================================

    // discesa dalla strada principale
    for(int y = 42; y <= 45; y++)
    {
        Griglia[y][115] = 2;
        Griglia[y][116] = 2;
    }

    // prima curva
    for(int y = 46; y <= 49; y++)
    {
        Griglia[y][116] = 2;
        Griglia[y][117] = 2;
    }

    Griglia[45][115] = 2;
    Griglia[45][116] = 2;
    Griglia[45][117] = 2;

    // seconda curva
    for(int y = 50; y <= 53; y++)
    {
        Griglia[y][117] = 2;
        Griglia[y][118] = 2;
    }

    Griglia[49][116] = 2;
    Griglia[49][117] = 2;
    Griglia[49][118] = 2;


    // =====================================================
// INGRESSO / AFFACCIO AL LAGHETTO
// =====================================================

for(int x = 118; x <= 123; x++)
{
    Griglia[54][x] = 2;
    Griglia[55][x] = 2;
}

// raccordo più naturale verso il pontile
Griglia[55][123] = 2;
Griglia[55][124] = 2;

Griglia[56][122] = 2;
Griglia[56][123] = 2;
Griglia[56][124] = 2;
Griglia[56][125] = 2;

// pontile dentro il lago
for(int x = 124; x <= 128; x++)
{
    Griglia[57][x] = 2;
    Griglia[58][x] = 2;
}


    // =====================================================
    // PIAZZOLA PANORAMICA DEL LAGO
    // =====================================================

    for(int y = 58; y <= 62; y++)
    {
        for(int x = 111; x <= 116; x++)
        {
            Griglia[y][x] = 2;
        }
    }

    // collegamento tra sentiero e piazzola
    for(int x = 111; x <= 118; x++)
    {
        Griglia[57][x] = 2;
    }


    // =====================================================
    // DECORAZIONI ATTORNO AL LAGHETTO
    // =====================================================

    // alberi
    Griglia[52][121] = 5;
    Griglia[53][124] = 5;

    Griglia[56][109] = 5;
    Griglia[63][111] = 5;

    Griglia[66][116] = 5;

    Griglia[70][122] = 5;
    Griglia[71][137] = 5;

    Griglia[66][146] = 5;
    Griglia[60][147] = 5;


    // rocce
    Griglia[53][129] = 4;
    Griglia[55][144] = 4;

    Griglia[64][117] = 4;

    Griglia[68][143] = 4;

    Griglia[62][109] = 4;


    // =====================================================
    // PANCHINA PANORAMICA AL LAGO
    // =====================================================

    Griglia[60][112] = 20;
    Griglia[60][113] = 20;


    // =====================================================
    // FIORI / VEGETAZIONE ATTORNO AL LAGO
    // =====================================================

    Griglia[56][118] = 17;
    Griglia[63][116] = 17;

    Griglia[69][120] = 17;
    Griglia[70][140] = 17;

    Griglia[57][145] = 17;


    // =====================================================
    // TORRE DI GUARDIA - NORD EST
    // =====================================================

    // piazzale
    for(int y = 27; y <= 32; y++)
    {
        for(int x = 127; x <= 149; x++)
        {
            Griglia[y][x] = 2;
        }
    }


    // =====================================================
    // SENTIERO VERSO LA TORRE
    // =====================================================

    for(int y = 37; y <= 40; y++)
    {
        Griglia[y][137] = 2;
        Griglia[y][138] = 2;
    }

    for(int y = 34; y <= 36; y++)
    {
        Griglia[y][136] = 2;
        Griglia[y][137] = 2;
    }

    Griglia[36][136] = 2;
    Griglia[36][137] = 2;
    Griglia[36][138] = 2;

    for(int y = 32; y <= 33; y++)
    {
        Griglia[y][137] = 2;
        Griglia[y][138] = 2;
    }

    Griglia[33][136] = 2;
    Griglia[33][137] = 2;
    Griglia[33][138] = 2;


    // =====================================================
    // TORRE - BASE
    // =====================================================

    for(int y = 24; y <= 28; y++)
    {
        for(int x = 131; x <= 145; x++)
        {
            Griglia[y][x] = 9;
        }
    }


    // =====================================================
    // TORRE - CORPO
    // =====================================================

    for(int y = 11; y <= 23; y++)
    {
        for(int x = 134; x <= 142; x++)
        {
            Griglia[y][x] = 9;
        }
    }


    // =====================================================
    // TORRE - PIETRE SCURE
    // =====================================================

    for(int y = 11; y <= 23; y++)
    {
        Griglia[y][134] = 10;
        Griglia[y][142] = 10;
    }

    for(int x = 131; x <= 145; x++)
    {
        Griglia[24][x] = 10;
        Griglia[28][x] = 10;
    }

    for(int y = 24; y <= 28; y++)
    {
        Griglia[y][131] = 10;
        Griglia[y][145] = 10;
    }


    // =====================================================
    // TORRE - MERLATURA
    // =====================================================

    for(int x = 133; x <= 143; x++)
    {
        Griglia[9][x] = 11;
    }

    Griglia[8][134] = 11;
    Griglia[8][137] = 11;
    Griglia[8][140] = 11;
    Griglia[8][143] = 11;

    for(int x = 134; x <= 142; x++)
    {
        Griglia[10][x] = 9;
    }


    // =====================================================
    // TORRE - FERITOIE
    // =====================================================

    Griglia[13][136] = 12;
    Griglia[13][140] = 12;

    Griglia[17][136] = 12;
    Griglia[17][140] = 12;

    Griglia[21][136] = 12;
    Griglia[21][140] = 12;


    // =====================================================
    // TORRE - PORTA
    // =====================================================

    Griglia[28][137] = 13;
    Griglia[28][138] = 13;


    // =====================================================
    // TORRE - SCALINI
    // =====================================================

    Griglia[29][136] = 2;
    Griglia[29][137] = 2;
    Griglia[29][138] = 2;
    Griglia[29][139] = 2;

    Griglia[30][135] = 2;
    Griglia[30][136] = 2;
    Griglia[30][137] = 2;
    Griglia[30][138] = 2;
    Griglia[30][139] = 2;
    Griglia[30][140] = 2;


    // =====================================================
    // RECINZIONI TORRE
    // =====================================================

    for(int x = 124; x <= 130; x++)
    {
        Griglia[27][x] = 1;
    }

    for(int y = 27; y <= 31; y++)
    {
        Griglia[y][124] = 1;
    }

    for(int x = 146; x <= 152; x++)
    {
        Griglia[27][x] = 1;
    }

    for(int y = 27; y <= 31; y++)
    {
        Griglia[y][152] = 1;
    }


    // =====================================================
    // ROCCE VICINO ALLA TORRE
    // =====================================================

    Griglia[18][127] = 4;
    Griglia[22][129] = 4;
    Griglia[15][148] = 4;
    Griglia[23][149] = 4;

    Griglia[32][130] = 4;
    Griglia[32][146] = 4;


    // =====================================================
    // ALBERI VICINO ALLA TORRE
    // =====================================================

    Griglia[6][119] = 5;
    Griglia[12][126] = 5;
    Griglia[6][140] = 5;
    Griglia[13][151] = 5;
    Griglia[19][153] = 5;

    Griglia[34][124] = 5;
    Griglia[34][149] = 5;


    // =====================================================
    // ALBERI SPARSI
    // =====================================================

    Griglia[5][8] = 5;
    Griglia[7][14] = 5;
    Griglia[10][7] = 5;
    Griglia[13][15] = 5;
    Griglia[16][9] = 5;
    Griglia[20][14] = 5;
    Griglia[23][8] = 5;
    Griglia[27][17] = 5;

    Griglia[5][31] = 5;
    Griglia[8][43] = 5;
    Griglia[6][58] = 5;
    Griglia[12][70] = 5;
    Griglia[17][84] = 5;
    Griglia[21][76] = 5;
    Griglia[16][91] = 5;

    Griglia[7][119] = 5;
    Griglia[10][127] = 5;
    Griglia[32][140] = 5;

    Griglia[25][82] = 5;
    Griglia[29][88] = 5;
    Griglia[33][81] = 5;
    Griglia[36][91] = 5;

    Griglia[57][8] = 5;
    Griglia[61][15] = 5;
    Griglia[65][9] = 5;
    Griglia[68][20] = 5;
    Griglia[72][13] = 5;
    Griglia[75][27] = 5;

    Griglia[58][39] = 5;
    Griglia[62][46] = 5;
    Griglia[67][38] = 5;
    Griglia[72][53] = 5;
    Griglia[75][61] = 5;

    Griglia[48][86] = 5;
    Griglia[52][91] = 5;
    Griglia[56][83] = 5;
    Griglia[63][88] = 5;
    Griglia[69][94] = 5;

    Griglia[52][146] = 5;
    Griglia[58][151] = 5;
    Griglia[64][148] = 5;
    Griglia[71][151] = 5;
    Griglia[75][139] = 5;


    // =====================================================
    // PICCOLI GRUPPI DI ALBERI
    // =====================================================

    Griglia[19][30] = 5;
    Griglia[20][31] = 5;
    Griglia[21][32] = 5;

    Griglia[34][15] = 5;
    Griglia[35][16] = 5;
    Griglia[36][17] = 5;

    Griglia[47][24] = 5;
    Griglia[48][25] = 5;

    Griglia[52][29] = 5;
    Griglia[53][30] = 5;

    Griglia[44][84] = 5;
    Griglia[45][85] = 5;
    Griglia[46][86] = 5;


    // =====================================================
    // ROCCE SPARSE
    // =====================================================

    Griglia[11][28] = 4;
    Griglia[18][55] = 4;
    Griglia[22][93] = 4;
    Griglia[31][18] = 4;
    Griglia[34][85] = 4;
    Griglia[46][28] = 4;
    Griglia[51][82] = 4;
    Griglia[58][33] = 4;
    Griglia[63][57] = 4;
    Griglia[69][76] = 4;
    Griglia[73][102] = 4;
    Griglia[50][147] = 4;
    Griglia[70][146] = 4;


    // =====================================================
    // PONTE NORD
    // =====================================================

    for(int x = 102; x <= 110; x++)
    {
        Griglia[18][x] = 2;
        Griglia[19][x] = 2;
    }


    // =====================================================
    // PONTE CENTRALE
    // =====================================================

    for(int x = 95; x <= 103; x++)
    {
        Griglia[39][x] = 2;
        Griglia[40][x] = 2;
    }


    // =====================================================
    // PONTE SUD
    // =====================================================

    for(int x = 102; x <= 109; x++)
    {
        Griglia[59][x] = 2;
        Griglia[60][x] = 2;
    }


    // =====================================================
    // PARTENZA GIOCATORE
    // =====================================================

    for(int y = 5; y <= 10; y++)
    {
        for(int x = 5; x <= 10; x++)
        {
            Griglia[y][x] = 0;
        }
    }
}


int Map::GetCella(int r, int c)
{
    return Griglia[r][c];
}