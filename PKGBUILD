pkgname=terton
pkgver=1.0.0
pkgrel=1
pkgdesc="Terton - terminal game written in C"
arch=('x86_64')
url="https://github.com/bhaki18/terton"
license=('MIT')
makedepends=('gcc')
depends=('ncurses')
source=("$url/archive/refs/heads/main.tar.gz")
sha256sums=('SKIP')

build() {
    cd "$srcdir/terton-main"
    gcc main.c -o terton -lncurses
}

package() {
    install -Dm755 "$srcdir/terton-main/terton" \
        "$pkgdir/usr/bin/terton"
}