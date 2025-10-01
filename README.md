# TowerDefense

SFML 2.6 tabanlı, C++20 ile yazılmış veri odaklı kule savunma oyunu. Proje; menüler, seviye seçim ekranı, ayarlar, codex, 12 oynanabilir seviye, 20 kule, 20 düşman, durum etkileri, ekonomi ve basit bir seviye editörü içerir.

## Gereksinimler

* CMake 3.20+
* C++20 uyumlu derleyici (MSVC 19.30+, GCC 10+, Clang 12+)
* Sistemde yüklü SFML 2.6+ (find_package ile bulunur)

## Derleme Talimatları

### Windows (MSVC)
```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/bin/TowerDefense.exe
```

### Linux (g++/clang++)
```bash
sudo apt-get install libsfml-dev # SFML paketini yükleyin
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/bin/TowerDefense
```

### macOS (Clang)
```bash
brew install sfml
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/bin/TowerDefense.app/Contents/MacOS/TowerDefense
```

## Oynanış

* Ana menüden seviye seçim, ayarlar, codex veya editöre girebilirsiniz.
* Seviye seç ekranında kilitli seviyeler save.json tarafından yönetilir.
* Bir seviyeyi başlattığınızda 300 altın ve 20 can ile başlarsınız (balance.json ile ayarlanır).
* Build alanlarına tıklayarak kule yerleştirin. Varsayılan olarak Arrow Mk.I açılır.
* `P` ile duraklatın, `1/2/3` tuşları ile oyun hızını 1x/2x/3x yapın.
* Dalga tamamlandığında otomatik bonus altın kazanırsınız.
* Codex ekranında tüm kule ve düşman istatistiklerini inceleyin.
* Seviye editörü (E ile export) yeni grid verisi üretir.

## Denge Ayarı

### `data/towers.json`
* `id`, `name`, `role`, `cost`, `damage`, `fireRate`, `range`, `projectileType`, `tags`: temel parametreler.
* `armorPen`, `aoeRadius`, `statusEffect`, `chain`, `pierce`, `income` isteğe bağlıdır.
* `upgrades` dizisi her seviye için delta değerleri içerir (ör. `{"lvl":2, "damage":6}`).
* `branchA` / `branchB`: iki dal için ek bonuslar ve açıklama.

### `data/enemies.json`
* `hp`, `speed`, `armor`, `magicResist`, `reward`, `abilities`, `tags` alanları düşman özelliklerini tanımlar.

### `data/waves.json`
* Her seviye için dalga listeleri (`spawnInterval`, `enemies`). `delay` alanı saniye cinsinden zamanlamayı kontrol eder.

### `data/balance.json`
* `global`: başlangıç can/altın ve genel çarpanlar.
* `difficultyCurve`: seviyeye göre HP/Hız/Ödül çarpanları (hp, speed, reward).
* `status`: durum etkilerinin süresi ve DPS değerleri.
* `economy`: öldürme bonusu, dalga bonusu ve satış geri ödemesi.

### `data/levels/level_XX.json`
* `grid`: genişlik/yükseklik/tile boyutu.
* `paths`: her poligon için koordinatlar (grid birimi).
* `buildable`: kule yerleştirilebilir hücreler.
* `obstacles`: engel hücreleri.
* `rules`: seviye açıklamaları (bilgi amaçlı).
* `waves`: bu seviyenin wave anahtarı.
* `startCoins`, `startLives`: seviye başlangıç değerleri.

### `data/settings.json` / `data/save.json`
* Ayarlar dosyası ses, hız, kalite ve renk modu içerir.
* Save dosyası açılmış kuleleri, son seviyeyi ve kazanılan rozetleri saklar.

## Dosya Yapısı
```
TowerDefense/
  assets/           # Yer tutucu görsel, ses ve font
  data/             # Oyun denge verileri ve seviyeler
  src/              # C++ kaynak kodu (core, ecs, systems, entities, ui, levels)
  vendor/include/   # nlohmann::json tek başlık implementasyonu
  CMakeLists.txt
  README.md
```

## Lisans Notları

* `assets/fonts/DejaVuSans.ttf` – Bitstream Vera türevi, [DejaVu Fonts](https://dejavu-fonts.github.io/) lisansı (public domain benzeri).
* `assets/audio/placeholder.wav` – Bu projede oluşturulan sessiz örnek, herhangi bir lisans kısıtı yoktur.
* `assets/textures/placeholder.png` – Tek piksel yer tutucu, bu projede üretilmiştir.

## Seviye Editörü

Ana menüden **Level Editor** seçeneğine girerek 16x12'lik bir grid üzerinde farenizle yol (1) ve inşa alanı (2) işaretleyebilirsiniz. `E` tuşu mevcut durumu `editor_export.json` olarak kaydeder; elde edilen veriyi `data/levels` altında kendi düzenlemeleriniz için kullanabilirsiniz.

