#include <flash/flash.h>
#include <flash/part.h>
#include <net/net.h>
#include <font/font.h>

static const char banner[] = "\n\n" // CLRSCREEN
	"\t+---------------------------------+\n"
	"\t|    Welcome to MaxWit g-bios!    |\n"
	"\t|      http://www.maxwit.com      |\n"
	"\t|      "__DATE__", "__TIME__"      |\n"
	"\t+---------------------------------+\n"
#ifdef CONFIG_DEBUG
	"\ng-bios running "
#ifdef CONFIG_IRQ_SUPPORT
	"with IRQ enabled!\n"
#else
	"in polling mode!\n"
#endif
#endif
	;

int exec_shell(void);

static int __INIT__ sys_init(void)
{
	int count = 1;
	init_func_t *init_call = init_call_begin;
	const char* func_name;

	while (init_call < init_call_end)
	{
		int ret;

		printf("%d. [0x%08x]", count, *init_call);
		func_name = get_func_name(*init_call);
		if(func_name)
		{
			printf(" %s()", func_name);
		}
		putchar('\n');

		ret = (*init_call)();
#ifdef CONFIG_DEBUG
		if (ret < 0)
			puts("Failed!");
		else
			puts("OK!");
#endif

		init_call++;
		count++;

		printf("\n");
	}

	printf("(g-bios initialization finished.)\n");

	return 0;
}

// fixme
static void __INIT__ show_sys_info(void)
{
	printf("%s\n", banner);

	net_check_link_status();
}

static int __INIT__ font_init(void)
{
	int count = 1;
	font_init_t *init_call = font_init_begin;
	const char* func_name;

	while (init_call < font_init_end)
	{
		int ret;

		printf("%d. [0x%08x]", count, *init_call);
		func_name = get_func_name(*init_call);
		if(func_name)
		{
			printf(" %s()", func_name);
		}
		putchar('\n');

		ret = (*init_call)();
#ifdef CONFIG_DEBUG
		if (ret < 0)
			puts("Failed!");
		else
			puts("OK!");
#endif

		init_call++;
		count++;

		printf("\n");
	}

	printf("(g-bios init fonts finished.)\n");

	return 0;
}

int main(void)
{
	sys_init();

	font_init();

	show_sys_info();

	printf("\n");
	exec_shell();

	return -1;
}
