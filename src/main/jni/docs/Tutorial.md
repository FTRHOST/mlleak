# Panduan Pembuatan Mod (MLBS Template)

Dokumentasi ini menjelaskan cara kerja sistem mod ini dan bagaimana Anda bisa menambahkan fitur baru dengan mudah.

## 1. Struktur Utama
- **`main.cpp`**: Otak dari mod. Mengatur thread utama, membaca konfigurasi, dan memanggil fungsi inisialisasi fitur.
- **`PathManager.cpp/h`**: Mengatur lokasi file secara otomatis agar mod bisa berjalan di berbagai versi Android tanpa hardcode path.
- **`feature/`**: Folder tempat logika mod berada. Setiap fitur harus memiliki file `.h` sendiri agar kode tetap bersih.

## 2. Alur Kerja Mod (Lifecycle)
1. **Entry Point**: Saat game memuat library (`.so`), fungsi `Initializer()` dipanggil secara otomatis.
2. **Stealth Thread**: Mod membuat thread baru agar tidak mengganggu (freeze) loading game.
3. **Il2Cpp Attach**: Mod menunggu hingga game memuat `libil2cpp.so` (mesin utama game).
4. **Config Polling**: Mod mengecek file `config.json` setiap 3 detik. Jika ada perubahan (misal: fitur diaktifkan dari luar), mod akan mengetahuinya tanpa perlu restart game.
5. **Hooking**: Mod mengganti fungsi asli game dengan fungsi buatan kita menggunakan `DobbyHook`.

## 3. Cara Menambahkan Fitur Baru
Misalkan Anda ingin membuat mod "Drone View":

1. **Buat File Baru**: Buat `src/main/jni/feature/DroneView.h`.
2. **Definisikan Hook**: Cari offset fungsi kamera di `dump.cs`.
3. **Gunakan Template**: Gunakan pola yang ada di `Template.h`.
4. **Daftarkan di main.cpp**:
   - Tambahkan `#include "feature/DroneView.h"`
   - Panggil `DroneView::Init()` di dalam fungsi `ApplyFeatures()`.

## 4. Opsi Konfigurasi Global
... (konten sebelumnya) ...

## 5. Memahami Tipe-Tipe Hooking
Memilih tipe data yang tepat sangat penting agar game tidak crash:

### A. Boolean (`bool`)
- **Kegunaan**: Fitur On/Off.
- **Contoh**: `get_IsVisible`, `get_IsDead`, `IsInBattle`.
- **Tips**: Jika Anda ingin fitur "Always On", cukup `return true;`.

### B. Void (`void`)
- **Kegunaan**: Fungsi yang melakukan aksi tapi tidak memberi jawaban balik.
- **Contoh**: `Update()`, `Attack()`, `OnCreate()`.
- **Tips**: Selalu panggil fungsi aslinya (`oFunction(...)`) di dalam hook agar alur game tetap berjalan normal.

### C. Numeric (`int`, `float`, `long`)
- **Kegunaan**: Nilai angka.
- **Contoh**: `get_Hp`, `get_Speed`, `get_Gold`.
- **Tips**: Berhati-hatilah mengembalikan nilai yang terlalu besar (misal HP 999999) karena bisa memicu proteksi server.

## 6. Cara Mencari Offset di `dump.cs`
Untuk membuat hook, Anda perlu data dari file `dump.cs`. Berikut cara membacanya:

### A. Mencari Alamat Method (Fungsi)
Cari nama fungsi yang Anda inginkan. Contoh:
```csharp
// Namespace: 
public class SystemData {
    public bool IsForbidSkin(int skinId); // RVA: 0x1234567 Offset: 0x1234567 VA: 0x1234567
}
```
- **Image**: Biasanya `Assembly-CSharp.dll` (untuk logika game).
- **Namespace**: Kosongkan `""` jika tidak ada.
- **Class**: `SystemData`
- **Method**: `IsForbidSkin`
- **Args**: `1` (karena ada `int skinId` di dalam kurung).

### B. Mencari Alamat Field (Variabel)
Contoh:
```csharp
public class TestGM {
    public bool debugMoreInfo; // 0x18
}
```
- Offset variabel `debugMoreInfo` adalah `0x18`. Gunakan ini di `Il2CppGetFieldOffset`.

## 7. Tips Efisiensi
- **Caching**: Jangan mencari offset setiap saat. Cari sekali, simpan di variabel `static`.
- **Minimal Parsing**: Gunakan `stat` (seperti yang sudah saya implementasikan di `main.cpp`) untuk mengecek apakah file konfigurasi benar-benar berubah sebelum membacanya kembali.
