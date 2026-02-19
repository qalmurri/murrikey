# ⌨️ Murrikey

**Murrikey** adalah aplikasi *screenkey* ringan untuk Linux (X11) yang ditulis menggunakan **C++** dan **Qt6**. Aplikasi ini menampilkan tombol yang kamu ketik secara real-time di layar, sangat cocok untuk demo koding, presentasi, atau pembuatan tutorial video.

## ✨ Fitur Utama

- **Real-time Overlay**: Menampilkan input keyboard (termasuk tombol kombinasi seperti Ctrl, Alt, Shift).
- **Customizable UI**: Ubah warna teks, jenis font, dan posisi vertikal langsung dari menu Settings.
- **Hide Duration**: Atur berapa lama teks muncul sebelum memudar (*fade-out*).
- **System Tray Integration**: Berjalan di latar belakang dengan menu akses cepat di top panel.
- **Lightweight**: Menggunakan memori yang sangat kecil dan efisien.

## 🚀 Cara Instalasi

### 1. Prasyarat (Dependencies)
Pastikan kamu sudah menginstal library yang dibutuhkan di Ubuntu/Debian kamu:

```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev libx11-dev libxkbfile-dev

git clone [https://github.com/qalmurri/murrikey.git](https://github.com/qalmurri/murrikey.git)
cd murrikey
mkdir build && cd build
cmake ..
make
