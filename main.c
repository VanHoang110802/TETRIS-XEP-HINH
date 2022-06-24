#include <cstdio>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <string.h>

#define LEFT 4
#define TOP 4
#define MaxI 22
#define MaxJ 10

int Board[MaxI][MaxJ];
char a[21][31];
int luachon = 1,phimnhan;
int capdo = 1, temp_capdo;
FILE *h;
int score = 0, highscore[5];
int level = 1;
float speed = 0.3;

void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize=20;
	SetConsoleCursorInfo (GetStdHandle (STD_OUTPUT_HANDLE), &Info);
}

// mình tự hỏi, mỗi chương trình game đều có những hàm hỗ trợ đều có cấu trúc không như nhau, phải chăng những hàm này đều là những hàm không có sẵn mà 1 lập trình viên nào đó đã dày công nghiên cứu được ra chăng?
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// hàm tham khảo thêm
void textcolor(WORD color)
{
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
void XoaManHinh()
{
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void Hinh()
{
    gotoxy(35,8); printf(" ______   ______     ______   ______     __     ______");
    gotoxy(35,9); printf("/\\__  _\\\ /\\  ___\\   /\\__  _\\ /\\  == \\   /\\ \\   /\\  ___\\");
    gotoxy(35,10); printf("\\/_/\\ \\/ \\ \\  __\\   \\/_/\\ \\/ \\ \\  __<   \\ \\ \\  \\ \\___  \\");
    gotoxy(35,11); printf("   \\ \\_\\  \\ \\_____\\    \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_\\  \\/\\_____\\");
    gotoxy(35,12); printf("    \\/_/   \\/_____/     \\/_/   \\/_/ /_/   \\/_/   \\/_____/");
}
void HinhNen()
{
    system("cls");
    textcolor(11);
    printf("\n");
    printf("\t\t/_/_/_/_/_/  /_/_/_/_/  /_/_/_/_/_/  /_/_/_/    /_/_/_/    /_/_/_/ \n");
    printf("\t\t   /_/      /_/            /_/      /_/    _/    /_/    /_/  \n");
    printf("\t\t  /_/      /_/_/_/        /_/      /_/_/_/      /_/      /_/_/ \n");
    printf("\t\t /_/      /_/            /_/      /_/    _/    /_/          /_/ \n");
    printf("\t\t/_/      /_/_/_/_/      /_/      /_/   /_/  /_/_/_/  /_/_/_/  \n");
    textcolor(15);
}
void HoanVi(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
void TaoFileCanThiet()
{
    h = fopen("dc.txt", "r");
    if (h == NULL)
    {
        h = fopen("dc.txt", "w");
        fclose(h);
    }
    luachon = 1;
}

void NhapDiem(const char *x)
{
    h = fopen(x, "r");
    for(int i =0;i<5;i++)
    {
        fscanf(h,"%d",&highscore[i]);
    }
    fclose(h);
}
void XuatDiem(const char *x)
{
    h = fopen(x, "w");
    for(int i =0;i<5;i++)
    {
        fprintf(h,"%d\n",highscore[i]);
    }

    fclose(h);
}
void ReSetBoard()
{
	int i,j;
	for(i=0;i<MaxI;i++)
    {
        for(j=0;j<MaxJ;j++)
        {
            Board[i][j]=0;
        }
    }
}
void DrawBoard()
{
    NhapDiem("dc.txt");
	int i,j;
	for(i=LEFT;i<=LEFT+10+1;i++)
    {
        for(j=TOP;j<=TOP+18+1;j++)
		{
			if((j==TOP||j==TOP+18+1)&&i>LEFT&&i<LEFT+10+1)
			{
				gotoxy(i,j);textcolor(7);printf("%c",196);
			}
			if((i==LEFT||i==LEFT+10+1)&&j>TOP&&j<TOP+18+1)
			{
				gotoxy(i,j);textcolor(7);printf("%c",179);
			}
		}
		gotoxy(LEFT+10+1,TOP);printf("%c",191);
		gotoxy(LEFT,TOP+18+1);printf("%c",192);
		gotoxy(LEFT+10+1,TOP+18+1);printf("%c",217);
		gotoxy(LEFT,TOP);printf("%c",218);
    }
}

void DisplayBoard() // nếu gạch chạm đến đáy thì dừng lại rồi in ra màn hình
{
	int i,j;
	for(i=0;i<MaxI;i++)
    {
        for(j=0;j<MaxJ;j++)
		{
			if(Board[i][j]==1&&i>=4)
			{
				gotoxy(j+LEFT+1,i+TOP+1-4);textcolor(15);printf("#");
			}
			if(Board[i][j]==0&&i>=4)
			{
				gotoxy(j+LEFT+1,i+TOP+1-4);printf(" ");
			}
		}
    }
}

void CapNhatLaiToaDo(int hang) //Khi xếp được 1 hàng đầy thì ta phải xóa hàng đó và cập nhật lại giá trị cho ma trân Board lại cho đúng.
{
	int i,j;
	for(i=hang;i>0;i--)
    {
        for(j=0;j<MaxJ;j++)
		{
			Board[i][j]=Board[i-1][j];
		}
    }
}

int CapNhat(int x)
{
    NhapDiem("dc.txt");

	score+=x;
	if(score>=200&&level<2)
	{
		level++;
		speed-=0.05;
	}

    if (score > highscore[4])
    {
        highscore[4] = score;
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 5; j++)
        {
            if (highscore[i] < highscore[j])
            {
                HoanVi(highscore[i], highscore[j]);
            }
        }
    }
	XuatDiem("dc.txt");
	return 0; //Chua win game.
}

void VeBangDiem()
{
    for(int i=16;i<=26;i++)
    {
        for(int j=4;j<10;j++)
		{
		    if((j==4||j== 9)&& i>16 && i<15+10+1)
			{
				gotoxy(i,j);textcolor(7);printf("%c",196);
			}
			if((i==16||i==15+10+1)&&j>4&& j<9)
			{
				gotoxy(i,j);textcolor(7);printf("%c",179);
			}
		}
		gotoxy(16,4);printf("%c",218);
		gotoxy(16,9);printf("%c",192);
		gotoxy(26,4);printf("%c",191);
		gotoxy(26,9);printf("%c",217);
    }
    for(int i=16;i<=32;i++)
    {
        for(int j=11;j<18;j++)
		{
		    if((j==11||j== 17)&& i>16 && i<15+15+2)
			{
				gotoxy(i,j);textcolor(7);printf("%c",196);
			}
			if((i==16||i==15+15+2)&&j>11&& j<18)
			{
				gotoxy(i,j);textcolor(7);printf("%c",179);
			}
		}
		gotoxy(16,11);printf("%c",218);
		gotoxy(16,17);printf("%c",192);
		gotoxy(32,11);printf("%c",191);
		gotoxy(32,17);printf("%c",217);
    }
	textcolor(14);
	//gotoxy(LEFT+MaxJ+3,13);printf("DIEM CAO: %d",diemcao);
	gotoxy(LEFT+MaxJ+3,12);printf("BEST: %d",highscore[0]);
	gotoxy(LEFT+MaxJ+3,13);printf("DIEM: %d",score);
	gotoxy(LEFT+MaxJ+3,14);printf("CAP DO: %d",level);
    gotoxy(LEFT+MaxJ+3,15);printf("TOC DO: %.3fs",speed);
    gotoxy(LEFT+MaxJ+3,16);printf("ESC: Exit");
}
typedef struct
{
	int **arr;
	int Row,Col;
	int iBoard,jBoard;
}KhoiGach;

KhoiGach *TaoKhoiGach(int ID)
{
	KhoiGach *pkhoigach=(KhoiGach*)malloc(sizeof(KhoiGach));
	switch(ID)
	{
		case 15:
			pkhoigach->Row=4;
			pkhoigach->Col=1;
			pkhoigach->iBoard=0;
			pkhoigach->jBoard=5;
			break;
		case 31:
			pkhoigach->Row=pkhoigach->Col=2;
			pkhoigach->iBoard=2;
			pkhoigach->jBoard=5;
			break;
		default:
			pkhoigach->Row=2;
			pkhoigach->Col=3;
			pkhoigach->iBoard=2;
			pkhoigach->jBoard=5;
			break;
	}
	pkhoigach->arr=(int**)malloc(pkhoigach->Row*sizeof(int*));
	for(int i=0;i<pkhoigach->Row;i++)
	{
		pkhoigach->arr[i]=(int*)malloc(pkhoigach->Col*sizeof(int));
	}
	for(int k=0;k<pkhoigach->Col*pkhoigach->Row;k++)
	{
		pkhoigach->arr[k/pkhoigach->Col][k%pkhoigach->Col]=(ID>>(pkhoigach->Col*pkhoigach->Row-1-k))&1;
	}
	return pkhoigach;
}
int Inside(int i,int j)
{
	return (i>=0&&i<MaxI&&j>=0&&j<MaxJ);
}
int Left(int i,int j)
{
	if(j>0&&Inside(i,j)&&Board[i][j-1]==0) return 1;
	return 0;
}
int Right(int i,int j)
{
	if(j<MaxJ-1&&Inside(i,j)&&Board[i][j+1]==0) return 1;
	return 0;
}
int Down(int i,int j)
{
	if(i<MaxI-1&&Inside(i,j)&&Board[i+1][j]==0) return 1;
	return 0;
}
void SangTrai(KhoiGach *pkhoigach)
{
	for(int i=0;i<pkhoigach->Row;i++)
    {
        for(int j=0;j<pkhoigach->Col;j++)
        {
            if(pkhoigach->arr[i][j]==1)
			{
				if(Left(pkhoigach->iBoard+i,pkhoigach->jBoard+j)==0||pkhoigach->iBoard<=3) return;
			}
        }
    }
	pkhoigach->jBoard-=1; //Dich vi tri cua bang trang thai sang trai 1 so voi Board[22][10].
}
void SangPhai(KhoiGach *pkhoigach)
{
	for(int i=0;i<pkhoigach->Row;i++)
	{
	    for(int j=0;j<pkhoigach->Col;j++)
        {
            if(pkhoigach->arr[i][j]==1)
			{
				if(Right(pkhoigach->iBoard+i,pkhoigach->jBoard+j)==0||pkhoigach->iBoard<=3) return;
			}
        }
	}
    pkhoigach->jBoard+=1; //Dich vi tri cua bang trang thai sang trai 1 so voi Board[22][10].
}
int RoiXuong(KhoiGach *pkhoigach)
{
	for(int i=0;i<pkhoigach->Row;i++)
    {
        for(int j=0;j<pkhoigach->Col;j++)
        {
            if(pkhoigach->arr[i][j]==1)
			{
				if(Down(pkhoigach->iBoard+i,pkhoigach->jBoard+j)==0) return 0;
			}
        }
    }
	pkhoigach->iBoard+=1; //Roi xuong 1 so voi Board[22][10]
	return 1;
}
void GanGiaTri(KhoiGach* pkhoigach)
{
	for(int i=0;i<pkhoigach->Row;i++)
    {
        for(int j=0;j<pkhoigach->Col;j++)
        {
            if(pkhoigach->arr[i][j]==1)
			{
				Board[pkhoigach->iBoard+i][pkhoigach->jBoard+j]=1;
			}
        }
    }
}
void XoayKhoiGach(KhoiGach* pkhoigach)
{
	int i,j;
	int ** tmpArr;
	int tmpRow=pkhoigach->Col;
	int tmpCol=pkhoigach->Row;
	//Cấp phát bộ nhớ cho ma trận phụ tmpArr.
	tmpArr=(int**)malloc(tmpRow*sizeof(int*));
	for( i=0;i<tmpRow;i++)
	{
		tmpArr[i]=(int*)malloc(tmpCol*sizeof(int));
	}

	for(i=pkhoigach->Row-1;i>=0;i--)
    {
        for(j=pkhoigach->Col-1;j>=0;j--)
		{
			tmpArr[j][pkhoigach->Row-i-1]=pkhoigach->arr[i][j];
		}
    }
	//Kiểm tra hợp lệ.
    for (i=0;i<tmpRow;i++)
    {
        for(j=0;j<tmpCol;j++)
        {
            if(!Inside(pkhoigach->iBoard+i,pkhoigach->jBoard+j)||Board[pkhoigach->iBoard+i][pkhoigach->jBoard+j]==1)
                return;
        }
    }
	for(i=0;i<pkhoigach->Row;i++)
    {
        free(pkhoigach->arr[i]);
    }
	free(pkhoigach->arr);
	//Cập nhật thay đổi sau khi xoay.
	pkhoigach->Col=tmpCol;
	pkhoigach->Row=tmpRow;
	pkhoigach->arr=tmpArr;
}
void HuyKhoiGach(KhoiGach* pkhoigach)
{
	int i;
	//Huy bo nho cua ma tran trang thai arr.
	for(i=0;i<pkhoigach->Row;i++)
    {
        free(pkhoigach->arr[i]);
    }
	free(pkhoigach->arr);
	//Sau do moi free(pkhoigach)
	free(pkhoigach);
	pkhoigach=NULL;
}
int KiemTra(KhoiGach *pkhoigach) //-1 : gameover 0: win
{
    int i,j,count;
    i=pkhoigach->Row-1;
    if(pkhoigach->iBoard<=3) {
            return -1;//Gameover
    }
    if(score>=1000) return 0;//Win

    while(i>=0)
    {
        count=0;
        for(j=0;j<MaxJ;j++)
        {
            if(Board[pkhoigach->iBoard+i][j]==1) count++;
        }
        if(count==MaxJ)
        {
            CapNhat(20);
            CapNhatLaiToaDo(pkhoigach->iBoard+i);
            DisplayBoard();
        }
        else
        {
            i=i-1;
        }
    }
    return 1;
}
void VeKhoiGach(KhoiGach* pkhoigach)
{
	int i;
	int j;
	for(i=0;i<pkhoigach->Row;i++)
    {
        for(j=0;j<pkhoigach->Col;j++)
        {
            if(pkhoigach->arr[i][j]==1&&(pkhoigach->iBoard+i)>3)
			{
				textcolor(12);
				gotoxy(LEFT+pkhoigach->jBoard+j+1,TOP+pkhoigach->iBoard+i-3);
				printf("#");
			}
        }
    }
}
void XoaKhoiGach(KhoiGach *pkhoigach)
{
	int i;
	int j;
	for(i=0;i<pkhoigach->Row;i++)
    {
        for(j=0;j<pkhoigach->Col;j++)
        {
            if(pkhoigach->arr[i][j]==1&&(pkhoigach->iBoard+i)>3)
			{
				gotoxy(LEFT+pkhoigach->jBoard+j+1,TOP+pkhoigach->iBoard+i-3);
				printf(" ");
			}
        }
    }
}
void Ve_Next(int ID)
{
	KhoiGach *pnext=TaoKhoiGach(ID);
	int iRoot=LEFT+MaxJ+5;
	int jRoot=TOP;
	for(int i=0;i<pnext->Row;i++)
	{
		for (int j=0;j<pnext->Col;j++)
		{
			if(pnext->arr[i][j]==1)
			{
				textcolor(11);
				gotoxy(iRoot+j+1,jRoot+i+1);printf("#");
			}
		}
	}
	HuyKhoiGach(pnext);
}
void Xoa_Next()
{
	int iRoot=LEFT+MaxJ+5;
	int jRoot=TOP;
	for(int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
            textcolor(0);
            gotoxy(iRoot+j+1,jRoot+i+1);printf(" ");
		}
	}
}
int Loai()
{
    int x=rand()%7;
	switch(x)
	{
	case 0:
		return 15;
		break;
	case 1:
		return 31;
		break;
	case 2:
		return 51;
		break;
	case 3:
		return 30;
		break;
	case 4:
		return 58;
		break;
	case 5:
		return 57;
		break;
	case 6:
		return 60;
		break;
	}
}
void LuuLaiDiemSo()
{
    system("cls");
    if (score <= highscore[4])
    {
        gotoxy(25, 6);
        printf("Diem cua ban: %d", score);
    }
    else if ((score > highscore[4]) && (score <= highscore[0]))
    {
        gotoxy(25, 6);
        printf("Diem cao: %d", score);
    }
    else
    {
        gotoxy(25, 6);
        printf("Diem cao nhat: %d", score);
    }
    textcolor(15);
    Sleep(1000);
}
void DatLai()
{
    system("cls");
    HinhNen();
    gotoxy(21,10); printf("Ban co chac muon dat lai?");
    gotoxy(21,11); printf("[c] Co");
    gotoxy(21,12); printf("[k] Khong");
    phimnhan = getch();
    if(phimnhan == 'c')
    {
        for (int i = 0; i < 5; i++)
        {
            highscore[i] = 0;
        }
        XuatDiem("dc.txt");
        system("cls");
        HinhNen();
        gotoxy(21,10); printf("THANH CONG!");
        Sleep(1000);
    }
}
int xuly()
{
    system("cls");
    Hinh();
    int IDKhoiTiepTheo;
    clock_t Start,End;
	char c;
	KhoiGach *currKhoi;
	currKhoi = TaoKhoiGach(Loai());
	IDKhoiTiepTheo = Loai();
	DrawBoard();
    while(1)
	{
		VeBangDiem();
		VeKhoiGach(currKhoi);
		Xoa_Next();
		Ve_Next(IDKhoiTiepTheo);
		Start=clock();
		while (float(End-Start)/CLK_TCK<speed)
		{
			if(kbhit())
			{
				c=(getch());
				XoaKhoiGach(currKhoi);
				if(c == 72)
                {
                    XoayKhoiGach(currKhoi);
                }
				else if(c == 75)
                {
                    SangTrai(currKhoi);
                }
				else if (c == 77)
                {
                    SangPhai(currKhoi);
                }
				else if (c == 80)
                {
                    RoiXuong(currKhoi);
                }
                else if( c == 27)
                {
                    ReSetBoard();   // cập nhật lại cho map chơi
                    LuuLaiDiemSo();
                    score =0;
                    level = 1;
                    speed = 0.3;
                    return 0;
				}
				VeKhoiGach(currKhoi);
			}
			End=clock();
		}
		XoaKhoiGach(currKhoi);
		if(RoiXuong(currKhoi)==0) //neu ma khoi gach cham dat
		{
			GanGiaTri(currKhoi);
			int ketqua=KiemTra(currKhoi);
			if((ketqua==-1||ketqua==0))
            {

                ReSetBoard();   // cập nhật lại cho map chơi
                LuuLaiDiemSo();
                score =0;
                level = 1;
                speed = 0.3;
                return 0;
            }
            HuyKhoiGach(currKhoi); //Giai phong bo nho.
			currKhoi = TaoKhoiGach(IDKhoiTiepTheo);
			IDKhoiTiepTheo = Loai();
		}
		DisplayBoard();
	}
	return 1;
}
void XemDiemCao()
{
    HinhNen();
    NhapDiem("dc.txt");
    gotoxy(5,9); printf("Bang xep hang diem cao:");
    for (int i = 0; i < 5; i++)
    {
        gotoxy(21, i + 11);
        printf("\t[TOP %d]: %d\n", i + 1, highscore[i]);
    }
    gotoxy(10,18); printf("Nhan bat ki de quay lai menu");
    getch();
}
void HuongDan()
{
    system("cls");
    HinhNen();
    gotoxy(5,9); printf("Tip huong dan nho:");
    gotoxy(10,11); printf("Tu tren troi se co xuat hien 1 khoi gach ngau nhien.");
    gotoxy(10,12); printf("Ban hay co gang kheo leo di chuyen chung ");
    gotoxy(10,13); printf("va sap xep chung thanh 1 buc tuong lon nhe.");
    gotoxy(10,14); printf("Khi sap xep duoc thanh 1 khoi day tuong nho.");
    gotoxy(10,15); printf("Ban se duoc cong them diem va qua level moi ne ><.");
    gotoxy(10,20); printf("Bam bat ki de quay lai menu");
    getch();
}
void Menu()
{
    XoaManHinh();
    HinhNen();
    if (luachon == 1)
    {
        gotoxy(22, 11);
        printf("%c", 16);
        textcolor(12);
        gotoxy(26, 11);
        printf("[1]: TRO CHOI MOI%1s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 11);
        printf("[1]: Tro choi moi%1s\u001b[0m\n", "");
    }
    if (luachon == 2)
    {
        gotoxy(22, 12);
        printf("%c", 16);
        textcolor(12);
        gotoxy(26, 12);
        printf("[2]: DIEM CAO%7s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 12);
        printf("[2]: Diem cao%7s\u001b[0m\n", "");
    }
    if (luachon == 3)
    {
        gotoxy(22, 13);
        printf("%c", 16);
        textcolor(12);
        gotoxy(26, 13);
        printf("[3]: HUONG DAN%7s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 13);
        printf("[3]: Huong dan%7s\u001b[0m\n", "");
    }

    if (luachon == 4)
    {
        gotoxy(22, 14);
        printf("%c", 16);
        textcolor(12);
        gotoxy(26, 14);
        printf("[4]: DAT LAI%5s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 14);
        printf("[4]: Dat lai%5s\u001b[0m\n", "");
    }
    if (luachon == 5)
    {
        gotoxy(22, 15);
        printf("%c", 16);
        textcolor(12);
        gotoxy(26, 15);
        printf("[5]: THOAT%8s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 15);
        printf("[5]: Thoat%8s\u001b[0m\n", "");
    }

    gotoxy(20, 20);
    printf("[%c] [%c]: Chon muc \t [enter]: Xac nhan", 30, 31);
    phimnhan = getch();
    if (phimnhan == 72)
    {
        luachon--;
        Sleep(10);
    }
    else if (phimnhan == 80)
    {
        luachon++;
        Sleep(10);
    }
    if (phimnhan == 13)
    {
        if (luachon == 1)
        {
            xuly();
        }
        else if (luachon == 2)
        {
            system("cls");
            XemDiemCao();
        }
        else if (luachon == 3)
        {
            HuongDan();
        }
        else if (luachon == 4)
        {
            DatLai();
        }
        else if (luachon == 5)
        {
            system("cls");
            exit(0);
        }
    }

    if (luachon < 1)
    {
        luachon = 5;
    }
    else if (luachon > 5)
    {
        luachon = 1;
    }
}
int main()
{
	srand(time(0));
	Nocursortype();
	TaoFileCanThiet();
    while (true)
    {
       Menu();
    }
	return 0;
}
