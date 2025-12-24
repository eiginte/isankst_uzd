# Egzamino (išankstinė) užduotis
# Aprašymas
Programa analizuoja tekstinį failą ir naudoja asociatyvius konteinerius (std::map, std::set) bei std::string, kad atliktų teksto analiz. Programa leidžia vartotojuo pasirinkti vieną iš dviejų rėžimų: žodžių analizę su cross-reference lentele arba URL adresų paiešką tekste. Programa veikia su bet kokiu vartotojo pasirinktu tekstiniu failu (.txt), kuris nurodomas programos paleidimo metu.

# Funkcionalumas 
Programa suteikia šias galimybes:

nuskaito tekstą iš vartotojo nurodyto .txt failo;

analizuoja tekstą eilutėmis;

ignoruoja skyrybos ženklus;

normalizuoja žodžius į mažąsias raides;

neįtraukia URL adresų į žodžių skaičiavimą;

suskaičiuoja, kiek kartų pasikartoja kiekvienas skirtingas žodis;

sukuria cross-reference tipo lentelę, kurioje nurodoma, kuriose teksto eilutėse kiekvienas žodis buvo paminėtas;

į rezultatų failą išveda tik tuos žodžius, kurie pasikartojo daugiau nei vieną kartą;

aptinka tekste esančius URL adresus (pilnus ir sutrumpintus);

užtikrina, kad URL adresai būtų unikalūs.

Rezultatai išvedami į failą (lentele.txt) arba į ekraną, priklausomai nuo pasirinkto režimo.

# Naudojami konteineriai
std::map<std::string, int> – žodžių pasikartojimų skaičiavimui;

std::map<std::string, std::set<int>> – eilučių numerių saugojimui cross-reference lentelei;

std::set<std::string> – unikalių URL adresų saugojimui;

std::string – teksto eilutėms ir žodžiams apdoroti.

# Kompiliavimas ir paleidimas
Programa kompiliuojama naudojant C++17 standartą:
g++ -std=c++17 main.cpp -o programa

Programa paleidžiama komanda:
./programa

Paleidimo metu vartotojas pasirenka norimą funkcionalumą ir nurodo analizuojamo teksto failą.

