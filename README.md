# Kalman Filter Project

Bu proje, bearing ölçümlerini kullanarak nesne takibi yapan gerçek zamanlı bir Kalman Filter implementasyonudur. Proje, simülasyon, filtreleme ve analiz olmak üzere üç ana bileşenden oluşmaktadır.

## 📋 İçindekiler
- [Proje Açıklaması](#proje-açıklaması)
- [Proje Yapısı](#proje-yapısı)
- [Kurulum](#kurulum)
- [Kullanım](#kullanım)
- [Makefile Rehberi](#makefile-rehberi)
- [Algoritma Detayları](#algoritma-detayları)
- [Rapor Oluşturma](#rapor-oluşturma)
- [Dosya Formatları](#dosya-formatları)

## 🎯 Proje Açıklaması

### Ana Amaç
Bu proje, 3 adet bearing sensörü kullanarak hareket eden bir nesnenin konumunu gerçek zamanlı olarak takip etmeyi amaçlar. Extended Kalman Filter (EKF) algoritması kullanılarak:

- **Konum tahmini** (x, y koordinatları)
- **Hız tahmini** (vx, vy bileşenleri)  
- **Gerçek zamanlı filtreleme**
- **Gaussian-Newton ile başlangıç konumu belirleme**

### Teknik Özellikler
- ✅ **3 Bearing Sensörü** ile triangulation
- ✅ **Extended Kalman Filter** (EKF) implementasyonu
- ✅ **Gaussian-Newton** optimizasyonu ile başlangıç konumu
- ✅ **Gerçek zamanlı veri işleme** (dosya takibi)
- ✅ **Matrix operasyonları** (4x4, 3x3, 2x2 matrisler)
- ✅ **Modüler kod yapısı** (simulation, kalman, analysis)

## 📁 Proje Yapısı

```
kalman-filter/
├── 📄 README.md              # Bu dosya
├── 📄 LICENSE                # Lisans bilgileri
├── 📄 Makefile               # Derleme ve çalıştırma komutları
├── 📁 inc/                   # Header dosyaları
│   ├── 📁 kalman/
│   │   ├── kalman.h         # Kalman filter tanımları
│   │   └── matrix.h         # Matrix operasyonları
│   └── 📁 simulation/
│       └── simulation.h     # Simülasyon tanımları
├── 📁 src/                   # Kaynak kodlar
│   ├── 📁 kalman/           # 🎯 Kalman Filter Modülü
│   │   ├── main.c           # Ana program (dosya takibi)
│   │   ├── kalman.c         # Kalman filter ana logic
│   │   ├── initialization.c # Matrix ve sensör başlatma
│   │   ├── parser.c         # Veri parsing
│   │   ├── gauss.c          # Gaussian-Newton optimizasyonu
│   │   ├── ekf.c            # Extended Kalman Filter
│   │   └── matrix_operations.c # Matrix işlemleri
│   ├── 📁 simulation/       # 🎮 Simülasyon Modülü
│   │   ├── main.c           # Simülasyon ana program
│   │   ├── initialization.c # Başlatma parametreleri
│   │   ├── record.c         # Veri kaydetme
│   │   └── write.c          # Dosya yazma işlemleri
│   └── 📁 analysis/         # 📊 Analiz Modülü
│       └── analysis.c       # Sonuç analizi ve karşılaştırma
├── 📁 report/               # 📝 LaTeX Raporları
│   ├── main.tex            # Ana rapor dosyası
│   ├── algorithm.tex       # Algoritma açıklamaları
│   ├── results.tex         # Sonuçlar ve grafikler
│   └── figures/            # Görsel dosyalar
├── 📁 bin/                  # 🚀 Derlenmiş executables (otomatik oluşur)
└── 📁 .obj/                 # 🔧 Object dosyaları (otomatik oluşur)
```

## 🛠 Kurulum

### Gereksinimler
- **GCC** compiler
- **Make** build sistemi
- **LaTeX** (raporlar için - opsiyonel)
  ```bash
  brew install --cask mactex  # macOS için
  ```

### Derleme
```bash
# Tüm projeyi derle
make

# Sadece belirli modülü derle
make simulation
make kalman
make analysis
```

## 🚀 Kullanım

### 1. Hızlı Başlangıç
```bash
# Tüm sistemi çalıştır (2 ayrı terminal penceresi açar)
make start
```

### 2. Adım Adım Çalıştırma
```bash
# 1. Simülasyonu çalıştır (veri üret)
./bin/simulation

# 2. Kalman filter'ı çalıştır (başka terminal'de)
./bin/kalman

# 3. Analiz yap (opsiyonel)
./bin/analysis
```

### 3. Veri Akışı
1. **Simulation** → `data.txt` dosyasına bearing ölçümleri yazar
2. **Kalman** → `data.txt` dosyasını gerçek zamanlı takip eder
3. **Analysis** → Sonuçları karşılaştırır

## 📖 Makefile Rehberi

### Ana Komutlar
| Komut | Açıklama |
|-------|----------|
| `make` | Tüm projeyi derler |
| `make start` | Simulation ve Kalman'ı ayrı terminallerde başlatır |
| `make simulation` | Sadece simülasyon modülünü derler |
| `make kalman` | Sadece kalman filter modülünü derler |
| `make analysis` | Sadece analiz modülünü derler |

### Rapor Komutları
| Komut | Açıklama |
|-------|----------|
| `make report` | LaTeX dosyalarından PDF oluşturur |

### Temizlik Komutları
| Komut | Açıklama |
|-------|----------|
| `make clean` | Object dosyalarını siler |
| `make fclean` | Object dosyları + binary'leri siler |
| `make re` | Temizlik yapıp yeniden derler |

### Yardım
```bash
make help  # Tüm komutları listeler
```

### Makefile Özellikleri
- ✅ **Akıllı derleme** - Sadece değişen dosyaları derler
- ✅ **Renkli çıktı** - Kolay takip için
- ✅ **Dependency tracking** - Header değişikliklerini takip eder
- ✅ **Temiz organizasyon** - Object dosyaları ayrı klasörde

## 🧮 Algoritma Detayları

### 1. Gaussian-Newton Başlatma
İlk 3 bearing ölçümü kullanılarak başlangıç konumu hesaplanır:

```c
// İlk 3 ölçüm topla
if (measurement_count < 3) {
    parse_data(data, &first_three[measurement_count]);
    measurement_count++;
    
    if (measurement_count == 3) {
        gauss_newton(kalman, first_three);  // Başlangıç konumu hesapla
    }
}
```

### 2. Extended Kalman Filter
4. ölçümden itibaren EKF döngüsü başlar:

**Predict Adımı:**
- State prediction: `x_k|k-1 = F * x_k-1|k-1`
- Covariance prediction: `P_k|k-1 = F * P_k-1|k-1 * F^T + Q`

**Update Adımı:**
- Innovation: `y = z - h(x_k|k-1)`
- Innovation covariance: `S = H * P_k|k-1 * H^T + R`
- Kalman gain: `K = P_k|k-1 * H^T * S^-1`
- State update: `x_k|k = x_k|k-1 + K * y`
- Covariance update: `P_k|k = (I - K * H) * P_k|k-1`

### 3. State Vector
```c
kalman->x[0] = x;      // X pozisyonu
kalman->x[1] = y;      // Y pozisyonu  
kalman->x[2] = vx;     // X hızı
kalman->x[3] = vy;     // Y hızı
```

### 4. Sensör Konfigürasyonu
```c
// 3 Bearing sensörü konumları
sensors[0] = {0.0, 0.0};      // Sensör 1: Orijin
sensors[1] = {500.0, 0.0};    // Sensör 2: X ekseni
sensors[2] = {250.0, 400.0};  // Sensör 3: Üçgen oluşturuyor
```

### 5. Noise Modeli
```c
// Process noise (Q matrix)
Q[0][0] = 0.1;  Q[1][1] = 0.1;   // Pozisyon noise
Q[2][2] = 1.0;  Q[3][3] = 1.0;   // Hız noise

// Measurement noise (R matrix)
R[0][0] = 0.5 * (π/180)²;   // Sensör 1: 0.5 derece
R[1][1] = 1.0 * (π/180)²;   // Sensör 2: 1.0 derece  
R[2][2] = 1.5 * (π/180)²;   // Sensör 3: 1.5 derece
```

## 📝 Rapor Oluşturma

### LaTeX Yapısı
```
report/
├── main.tex           # Ana rapor dosyası
├── algorithm.tex      # Algoritma açıklamaları
├── results.tex        # Sonuçlar ve analizler
├── references.bib     # Kaynaklar
└── figures/          # Grafikler ve şekiller
    ├── system_diagram.pdf
    ├── results_plot.pdf
    └── error_analysis.pdf
```

### PDF Oluşturma
```bash
# Tüm LaTeX dosyalarını derle
make report

# Manuel derleme (report/ klasöründe)
cd report
pdflatex main.tex
bibtex main
pdflatex main.tex
pdflatex main.tex
```

### Rapor İçeriği
1. **Giriş** - Proje amacı ve kapsamı
2. **Algoritma** - Kalman Filter ve Gaussian-Newton detayları
3. **Implementasyon** - Kod yapısı ve tasarım kararları
4. **Sonuçlar** - Performans analizi ve grafikler
5. **Sonuç** - Değerlendirme ve gelecek çalışmalar

## 📄 Dosya Formatları

### data.txt Format
```
timestamp sensor_id bearing
1.04 1 62.5321
2.53 2 120.7263
4.25 3 -150.1358
```

### Parser Detayları
```c
// Veri parsing örneği
parse_data("1.04 1 62.5321", &measurement);
// measurement.timestamp = 1.04
// measurement.sensor_id = 1
// measurement.bearing = 62.5321 (derece)
```

---
