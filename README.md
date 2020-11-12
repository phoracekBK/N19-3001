# N19-3001
Lepící linka Hanover



** TODO **
* modifikovat unit testy aby byly opět zpustitelné
* upravit unit testy aby všechny prošly (PASS)
* modifikovat ovladač pro roboty, vytvořit společný kód pro spouštění programů
* upravit program pc_queue, tak aby byl schopen komunikovat s PLC
* vytvořit jednotkové testy pro pc_queue
* implementovat DB pro komunikaci s MES
* vytvořit cyklus pro kontrolu live bitu pro implementaci v MES komunikaci (společné všem komunikačním cyklům)

* opravit dokončování lepení, aby si mohl manipulační robot dříve převzít
  sklo, je to na základě dokončení lepícího programu robota ne na základě
volného profilu
* majáky na signalizaci objemu sudu (blíží se konec sudu, prázdný sud)
* doplnit dva užívatelské účty - chybí název použivatelov a práva
* problém při porovnávání rear_window_type v počítání typů skel v sušícím regálu (prozatím vyřešeno, ale nutná oprava)




** DONE **
* modifikovat datovou strukturu pro komunikaci s programem pc_queue a modifikovat zdrojový kód aby byl zkompilovatelný
* cyklis pro roboty na referencování polohy
* upravit funkci na výpočet aplikovaného množství na základě převodového poměru čerpadel
* opravit předávání informací z lepícího stolu na gripper a výstupní stanici
* potvrzení estop pomocí tlačítek na výstupu
* chyba pri prechodu z manualu do automatu - vypina se Metralight
* opravit ukládání csv, nové jméno s aktuálním datem je vytvořeno pouze při spuštění a pro vytvoření jména pro další den je nutné program restartovat
* mixer vypršený čas na živ smesi se vyresetuje i životnost mixéru / upraveno
* na straně robotů připravit podmínku, že při odjezdu robota od skla musí zmizet signál přítomnosti skla jinak zopakovat přisátí a odsátí, aby nezůstalo sklo sporadicky přisáté (nebude implementováno)
* v případě, že je směs expirovaná, není žádný alarm (je alarm ale pouze když není suchý režim)
* když se vypne sama lepici technologie po odpuštení 3x, tak doplnit aspoň hlášku na displej
* prekrývání textového pole nebo tlačítka - chybové hlášky / upraveno
* chybaji popisky v HMI / upraveno
* přidat tabulku s informacemi o počtech skel v sušícím bufferu
  (implementován algoritmus součtu)
* životnost mixéru zobrazit v min, ne v s / upraveno
* upravit ukládání času posledního odpouštění nebo aplikace AB složky, je
  nutné uložit předchozí čas ne aktuální
* přidat hodnoty mezních hodnot intervalu teplot pro vytápění pistole a sudu
* změnit umístění hodnot pro aplikované množství lepidla, vložit přímo do
  struktury skla
* přemístit všechny zbývající informace do struktury skla pro lepší
  zaznamenávání informací pro csv v průběhu výroby
* cyklus pro roboty na test brzd
* upravit program pro generování csv souborů, aby pracoval dle požadavků
* inicializace estop stavu u robotů???
* odstranit duplicitní objekt primer_checker ze struktury carrousel!!!
* udělat překlad hmi
* doladit mazání záznamů skel z regálů sušícího bufferu
* reset error watchdog timerů v případě stopky
* přidat inicializaci pracovního cyklu
* otestovat kalibraci metralightu
* opravit proces kontroly primeru kdy je vyhodnocen jako nok a přesto projde
* opravit případ kdy je primovací proces vypnut a po nok kontrole primeru je sklo vráceno a šablona sjede dolů
* opravit případ kdy je ponechán lepící robot v údržbové pozici a je zahájen
  povel k lepení
* ošetřit stisknutí e-stopky
* přepojit časování automatického odpouštění na interní časovače v lepící
  technologii a přidat ocharnu proti odpouštění s mrtvým mixérem
* přidat aktivační tlačítko pro automatické odpouštění lepidla
* ošetřit aby se rotační stůl nemohl otočit v případě, že není v druhém
  centrovacím stole sklo a nebo je profil karuselu manipulačního robota ve
stavu false
* ošetřit aby clony karuselu byly forcovány do horní polohy pokud je profil
  karuselu manipulačního robota ve stavu false
* opravit měření objemu lepidla v lepicí technologii
* přidat resetování stavu pozice centrovací stůl 1 nebo 2 v primovací šablona naboře nebo dole v případě, že se
  stůl nebo primovací šablona otáčí do opačné pozice, nebo je s ním manuálně hýbáno, pro případ, že
by došlo k estop v průběhu otáčení a v plc by zůstal viset signál, že je stůl
nebo šablona stále v koncové pozici
* manuální ovládání clony, UV světel a kamer na vyhodnocení primeru
* přidána funkce pro vypnutí řídících obvodů na tlačítko stopp na hlavním
  rozvaděči
* nastavit přídavnou síťovou kartu pro komunikaci s MES (konfigurace IP adresy)
* upravit cyklus ukládání skla do bufferu, až ve chvíli kdy tam fyzicky je
  vloženo
* přidat dynamické nastavení cesty kam ukládat csv
* vymázání referencí skla z bufferu buď když je tam sklo tak jej vyloží na
  výstup bez aplikace lepidla, když tam není sklo tak pouze vymaže referenci
* přidat stav automat na vstupní a výstupní HMI
* vyhazování skel bez aplikace lepidla
* přidat na hmi stav lepící technologie start a informace na inicializaci
  vstupu a výstupu
* přidat informaci o stavu automat/manual do hlavního hmi
* upravit ukládání referenci skla do bufferu
* bezpečnost na karuselu na základě optické brány
* stavove nastavení volných profilů
* upravit zavírání clon na primovací kontrolu



** změny v csv **
* Vehicle number (nur von MES system) - unikátní číslo identifikující dveře
* Rear window type (ohne MES -> Operator wählt es durch HMI aus) - typ skla
* Model - model dveří na které bude sklo lepeno
* ID number (die Klebeanlage generiert) - vnitřní ID skla
* Time of primer application - čas aplikace primeru, pokud není aplikováno je
  hodnota NA
* Primer Detection with Vision OK - výsledek kontroly primeru, pokud je
  vypnuto, tak hodnota NA
* Bead Check pass/fail area - zóny, které byly kamerami na kontrolu primeru
  vyhodnoceny jako nok, ale nok neprojdou od stanice, pokud je kontorla
vypnuta tak je hodnota NA
* Primer Curing Rack - index regálu kde bylo sklo uloženo ke zrání
* Timestamp when Primer Flashoff Completed - čas kdy byl primer na skle
  vyzrály k aplikaci lepidla (pouze pokud je primováno jinak je hodnota NA)
* Time interval from primer application until glue application ends - čas od
  aplikace primeru do ukončení aplikace lepidla (pouze pokud je primováno,
jinak je hodnota NA)
* Time since last dispense Timestamp when gluebed was started to applied - čas
  poslední aplikace lepidla nebo odpouštění složky AB
* Timestamp of glue bead application - čas kdy robot začne nanášet lepidlo
* Robot Finish Time glue bead application - čas kdy robot dokončil nanášení
  lepidla
* Time interval from glue bead application until glass is taken out from
  output station - interval od začátku aplikace do vyjmutí ze stanice
* Barrel Expiry OK "A" - expirace sudu složky A ok/nok
* Adhesive Batch ID "A" - produktové číslo sudu složky A
* Adhesive Serial # "A" - sériové číslo sudu složky A
* Material Dispensed "A" [ml] - aplikované množství složky A
* Pistol Temperature Actual "A" - aktuální teplota pistole při aplikaci
* Pot Temperature Actual "A" - aktuální teplota sudu složky A
* Barrel Expiry OK "B" - expirace sudu složky B ok/nok
* Adhesive Batch ID "B" - produktové číslo sudu složky B
* Adhesive Serial # "B" - sériové číslo sudu složky B
* Material Dispensed "B" [ml] - aplikované množství složky B
* Mischungsverhältnis Komponente A zu B - aplikační poměr nanášeného lepidla
* Mix tube life - životnost mixéru
* Robot Completed Cycle Without Fault - zda při pohybu manipulačního robota
  nebyla stisknuta estop
* Dispense Unit Completed Cycle Without Fault - zda při nanášení lepidla
  nebyla stisknuta estop
* Rotary Unit Completed Cycle Without Fault - zda nebyla v procesu otáčení
  carrouselu stisknuta estop
* Adhesive Application Process Complete Summary - zda nebyla
* Bead Check Camera Laser- Sensor - výsledek měření housenky metralight
* Bead Check Pass / Fail Area (Laser- Sensor) - nelze vyčíst data ze senzoru metralight
