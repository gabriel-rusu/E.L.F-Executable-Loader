# Tema3: Loader de Executabile
## Nume & Prenume: Rusu Gabriel, 331AA

In implementarea temei ma ales se folosesc urmatoarele structuri:

```C
typedef struct page //structura pentru a reprezenta o pagina mapata
{
void *pageAddress;
struct page *nextPage;
} Page;

typedef struct list // structura ce tine evidenta tuturor paginilor mapate
{
Page *cachedPages;
int pageSize;
} Loader;
```
    In implementare am suprascris comportamentul default al handler-ului semnalului SIGSEGV pentru a reusi maparea in memorie a paginilor care nu au fost inca incarcate
.Paginile ce erau mapate de catre executabil au fost adaugate in lista cachedPages din structura Loader pentru a putea verifica posibile accesuri la memorie ce nu are premisiunile corespunzatoare.
**Mentiune:** Intregul enunt al temei a fost implentat.

## Rularea:
rularea temei se face prin executarea comenzi ```make``` in directorul radacina

## Bibliografie
    - laborator 4 SO
    - laborator 7 SO
    - laborator 6 So
    - cursul de SO
    - Modern Operating Systems book
