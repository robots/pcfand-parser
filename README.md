# Parser pre PC FAND datove subory

Tento parser bol napisany pri migracii databazy do niecoho novodobejsieho - XML. Vsetky informacie boli ziskane spatnym inzinierstvom.

Kedze datove subory PC fand neobsahuju ziadnu informaciu o strukture, tato musi byt dodana. Podobne ako v PC FAND. Syntax definicie struktur je podobny ako v samotnnom PC FAND, aby sa dala jednoducho skopirovat s malymi upravami.

Program potrebuje na vstupe SKRIPT.

## skript

Prvy riadok je cesta k vstupnemu suboru. Druhy riadok je cesta k vystupnemu suboru. Treti riadok je nazov XML-elementu. Zvysne riadky obsahuju strukturu dat.

Hlavny rozdiel oproti PC FAND je pridana dalsia ciarka na konci, a za nou nazov. Tento nazov, ak existuje, je pridany do XML elementu ako atrubut s hodnotou zo zaznamu.

## Build

Na linuxe standardne prikazom "make".

Teoreticky sa to da zbuildovat aj s mingw gcc. 

## Stav

Projekt bol testovany na 32bitovom linuxe ako migracia dat.

## Licencia

Robte si s tym co chcete, ale spomente pri tom moje meno. Samozrejme nezodpovedam za spravnost programu. Podpora ziadna.


## PS

Zdrojaky PC fandu boli zverejnene firmou ALIS na ich githube: https://github.com/alisoss/pcfand

