// структура описания заголовка раздела
struct __attribute__ ((__packed__)) pheader {
 uint32_t magic;    //   0xa55aaa55
 uint32_t hdsize;   // размер заголовка
 uint32_t hdversion;
 uint8_t unlock[8];
 uint32_t code;     // тип раздела
 uint32_t psize;    // разме поля данных
 uint8_t date[16];
 uint8_t time[16];  // дата-время сборки прошивки
 uint8_t version[32];   // версия пршоивки
 uint16_t crc;   // CRC заголовка
 uint32_t blocksize;  // размер блока CRC образа прошивки
}; 


// Структура описания таблицы разделов


struct ptb_t{
  unsigned char pname[20];    // буквенное имя раздела
  struct pheader hd;  // образ заголовка
  uint16_t* csumblock; // блок контрольных сумм
  uint8_t* pimage;   // образ раздела
};

//**********************************************************
//*  Класс для работы с таблицей разделов
//**********************************************************

class ptable_list {
  // хранилище таблицы разделов
  struct ptb_t table[120];
  int npart; // число разделов в таблице
  
public:
  // конструктор
  ptable_list() { npart=0; }
  // деструктор
  ~ptable_list() {clear();}
  // извлечение разделов из файла 
  void extract(FILE* in);  
  // очистка всей таблицы
  void clear();
  // получение размера таблицы
  int index() {return npart; }
  // получение размера заголовка
  uint32_t crcsize(int n) { return table[n].hd.hdsize-sizeof(pheader); }
  // получение размера образа
  uint32_t psize(int n) { return table[n].hd.psize; }
  // получение кода раздела
  uint32_t code(int n) { return table[n].hd.code; }
  // получение имени раздела
  uint8_t* name(int n) { return table[n].pname; }
  // получение ссылки на заголовок
  struct pheader* hptr(int n) { return &table[n].hd; }
  // получение ссылки на образ раздела
  uint8_t* iptr(int n) { return table[n].pimage; }

  // получение ссылок на описательные поля заголовка
  uint8_t* platform(int n) { return table[n].hd.unlock; }
  uint8_t* date(int n) { return table[n].hd.date; }
  uint8_t* time(int n) { return table[n].hd.time; }
  uint8_t* version(int n) { return table[n].hd.version; }
  
  
  void findparts(FILE* in);
  void loadimage(int np, FILE* in);
  void save_part(int n,FILE* out);
  void calc_crc16(int n);
  void calc_hd_crc16(int n); 

  // удаление раздела
  void delpart(int n);
  // перемещение вверх
  void moveup(int n);
  // перемещение вниз
  void movedown(int n);  
};
  

extern ptable_list* ptable;  
  
void  find_pname(unsigned int id,unsigned char* pname);