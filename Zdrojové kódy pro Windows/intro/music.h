#ifndef _MUSIC_H_
#define _MUSIC_H_

/**
 * @brief Inicializuje hudbu
 *
 * @param Music hudba
 */
void music_Init(unsigned char*Music);

/**
 * @brief Zacne prehravat hudbu
 */
void music_Play();

/**
 * @brief Deinicializuje hudbu
 */
void music_DeInit();

#endif//_MUSIC_H_
