/*
 * File: process_init.c
 * Author: Joel Souza
 * Date: 2026-07-30
 * Description: Functions for each phase of process initialization
 */

extern	void (*__start_init_array[])(void);
extern	void (*__stop_init_array[])(void);

/*
 * TODO:
 * */
void	process_init_array(void)
{
	void	(**fn_startup)(void);

	fn_startup = __start_init_array;
	while (fn_startup < __stop_init_array)
		(*fn_startup++)();
}
