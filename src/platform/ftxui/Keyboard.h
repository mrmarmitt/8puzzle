#pragma once

// Leitura de teclado NÃO-bloqueante para a plataforma FTXUI.
// Diferente do terminal clássico (que bloqueia em _getch e "paceia" o loop por
// acidente), aqui o loop da cengine gira livre — o pacing é do WindowManager.
enum class Key {
    None, // nenhuma tecla pendente
    Up,
    Down,
    Left,
    Right,
    Enter,
    Escape,
    Other
};

// Consome no máximo uma tecla por chamada (trata o prefixo 0/224 das setas).
Key readKey();
