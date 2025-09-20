# TowerDefense

SFML 2.6 tabanlı, C++20 ile yazılmış veri odaklı kule savunma oyunu. Proje; menüler, seviye seçim ekranı, ayarlar, codex, 12 oynanabilir seviye, 20 kule, 20 düşman, durum etkileri, ekonomi ve basit bir seviye editörü içerir.

## Gereksinimler

* CMake 3.20+
* C++20 uyumlu derleyici (MSVC 19.30+, GCC 10+, Clang 12+)
* İnternet erişimi (CMake FetchContent ile SFML 2.6.0 indirir) veya sistemde yüklü SFML 2.6+
* Çevrimdışı/CI senaryoları için varsayılan olarak etkin gelen minimal `USE_SFML_STUB=ON` yapı seçeneği (yalnızca temel render stub'ı, gerçek oynanış için `OFF` yapın)

## Derleme Talimatları

### Windows (MSVC)
```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/bin/TowerDefense.exe

# Gerçek SFML kullanmak için (önceden kurulmuşsa)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUSE_SFML_STUB=OFF
cmake --build build --config Release
./build/bin/TowerDefense.exe
```

### Linux (g++/clang++)
```bash
sudo apt-get install libsfml-dev # sistem paketini tercih ederseniz
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/bin/TowerDefense

# Gerçek SFML ile derlemek için
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUSE_SFML_STUB=OFF
cmake --build build -j
./build/bin/TowerDefense
```

### macOS (Clang)
```bash
brew install sfml
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/bin/TowerDefense.app/Contents/MacOS/TowerDefense

# Gerçek SFML ile derlemek için
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUSE_SFML_STUB=OFF
cmake --build build
./build/bin/TowerDefense.app/Contents/MacOS/TowerDefense
```

### CLion ile Derleme ve Çalıştırma

1. **Projeyi Açın:** `File → Open` menüsünden bu dizini seçin. CLion, CMake projesini otomatik olarak algılar.
2. **Toolchain Ayarları:** `Settings/Preferences → Build, Execution, Deployment → Toolchains` bölümünden ilgili platforma uygun derleyiciyi (MSVC, MinGW, GCC veya Clang) seçin. Gerekirse CMake ve Ninja yollarını güncelleyin.
3. **CMake Profili:** `Settings/Preferences → Build, Execution, Deployment → CMake` sekmesinde `Release` ve `Debug` profilleri oluşturun. `CMAKE_BUILD_TYPE` değerini `Release` olarak ayarlarsanız oyun varsayılan olarak -O2 ile derlenir.
4. **SFML Seçimi:** Varsayılan CMake profili `USE_SFML_STUB=ON` olarak gelir; bu seçenek minimal stub ile offline derlemeyi sağlar. Gerçek SFML grafik/ses desteği için profilde `-DUSE_SFML_STUB=OFF` ekleyin ve SFML 2.6+ kurulum yolunu (gerekirse `SFML_DIR`) belirtin. FetchContent kullanacaksanız ilk yapı sırasında kaynak indirilir.
5. **Yapılandırma:** Proje açıldığında CLion sağ alt köşede CMake yapılandırmasını tamamlar. Gerekirse `Reload CMake Project` seçeneğini kullanın.
6. **Çalıştırma Yapılandırması:** `Run → Edit Configurations` menüsünden yeni bir `CMake Application` oluşturun. `Target` alanı için `TowerDefense` seçin, `Executable` yolunun `cmake-build-*/bin/TowerDefense[.exe]` olduğundan emin olun.
7. **Çalıştır / Hata Ayıkla:** `Run` (`Shift+F10`) veya `Debug` (`Shift+F9`) komutlarıyla oyunu başlatabilirsiniz. Çalışma dizini olarak proje kökünü bırakmak, `data/` ve `assets/` dosyalarının doğru yüklenmesini sağlar. Stub ile derlerseniz pencere yerine basitleştirilmiş döngü çalışır; gerçek oynanış için SFML modunu açın.

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
  assets/           # Yer tutucu açıklama dosyaları; oyun eksik asset olduğunda otomatik debug görselleri kullanır
  data/             # Oyun denge verileri ve seviyeler
  src/              # C++ kaynak kodu (core, ecs, systems, entities, ui, levels)
  vendor/include/   # nlohmann::json tek başlık implementasyonu
  CMakeLists.txt
  README.md
```

## Lisans Notları

* Depoda yalnızca `assets/*/README.txt` dosyaları bulunur. Bunlar, kendi font/görsel/ses dosyalarınızı nereye eklemeniz gerektiğini açıklar.
* Hiçbir ikili asset yer almıyor; SFML varsayılan fontu ve motorun ürettiği sessiz ses arabelleği ile dama desenli debug dokusu kullanılır.

## Seviye Editörü

Ana menüden **Level Editor** seçeneğine girerek 16x12'lik bir grid üzerinde farenizle yol (1) ve inşa alanı (2) işaretleyebilirsiniz. `E` tuşu mevcut durumu `editor_export.json` olarak kaydeder; elde edilen veriyi `data/levels` altında kendi düzenlemeleriniz için kullanabilirsiniz.

