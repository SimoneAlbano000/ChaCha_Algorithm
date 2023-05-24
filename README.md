# ChaCha_Algorithm
ChaCha 20/12/8 Stream Cipher implementation for Embedded Devices

The 20-round Stream Cipher ChaCha20 is consistently faster (in software) than the Advanced Encrtyption Standard algorithm (AES) and is recommended for cryptographic applications where dedicated hardware acceleration is not present (for example a mid-low budget microcontroller). The reduced-round ciphers ChaCha12 and ChaCha8 are among the fastest 256-bit stream ciphers available and can be implemented in enviroments where computational speed is more important then security. 

### ChaCha20 performances on an ATMega2560 @ 16MHz clock

```
Computed digest: (with -Os flag)
RAM:   [          ]   4.6% (used 378 bytes from 8192 bytes)
Flash: [          ]   3.0% (used 7626 bytes from 253952 bytes)
Message: "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it."
162.76 H/s, in 0.012288 s

Computed digest: (with -O3 flag)
RAM:   [          ]   4.6% (used 378 bytes from 8192 bytes)
Flash: [-         ]   5.6% (used 14308 bytes from 253952 bytes)
Message: "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it."
164.74 H/s, in 0.012140 s
```

### Compile the code for windows
```
cd ~tests\tests_windows\

chacha.c:
    gcc -O3 chacha.c ../../lib/chacha.c -o chachatest.exe
```
