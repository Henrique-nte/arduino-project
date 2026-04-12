#ifndef SENSORES_LINHA_H
#define SENSORES_LINHA_H

void sensores_init(int eExt, int eInt, int dInt, int dExt);

void lerSensores(bool &eExt, bool &eInt, bool &dInt, bool &dExt);

#endif