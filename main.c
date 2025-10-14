#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Data Struct
typedef struct { int h, w, iter; char **grid; } Data;

// Initialize Data

char	**setup_grid(int h, int w)
{
	char **grid = calloc(h, sizeof *grid);
	if (!grid) return NULL;

	for(int i = 0; i < h; i++)
	{
		grid[i] = calloc(w, sizeof *grid[i]);
		if (!grid[i]) return NULL;

		for(int j = 0; j < w; j++)
			grid[i][j] = ' ';
	}
	return grid;
}

void	init_data(Data *d, char **av)
{
	d->w = atoi(av[1]);
	d->h = atoi(av[2]);
	d->iter = atoi(av[3]);
	d->grid = setup_grid(d->h, d->w);
}

// Populate Grid

void	populate_grid(Data *d)
{
	if (d->h <= 0 || d->w <= 0) return ;

	char		cmd;
	const char	up = 'w', down = 's', left = 'a', right = 'd', toggle = 'x';
	int			y = 0, x = 0, isWriting = 0;

	while (read(0, &cmd, 1) > 0)	
	{
		if (isWriting) d->grid[y][x] = 'O';
		switch (cmd)
		{
			case up: if (y > 0) y--; break;
			case down: if (y < d->h - 1) y++; break;
			case left: if (x > 0) x--; break;
			case right: if (x < d->w - 1) x++; break;
			case toggle: isWriting = !isWriting; break;
			default: break;
		}
	}
}


// Standard Output Grid

void	put_grid(char **grid, int h, int w)
{
	if (!grid || h <= 0) return ;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
			putchar(grid[i][j]);
		putchar('\n');
		
	}
}

// Free Grid Memory

void	free_grid(char **grid, int h)
{
	if (!grid || h <= 0) return ;

	for (int i = 0; i < h; i++)
		free(grid[i]);
	free(grid);
}

// Conway's Game of Life Rules Applied to Grid

int	count_neighbors(Data *d, int i, int j)
{
	int count = 0;
	
	for (int ios = -1; ios <= 1; ios++)
	{
		for (int jos = -1; jos <= 1; jos++)
		{
			if (ios == 0 && jos == 0) continue;		

			int y = i + ios, x = j + jos;
			if (y >= 0 && y < d->h && x >= 0 && x < d->w)
				if (d->grid[y][x] == 'O') count++;
		}
	}
	return count;
}

void	evolve_grid(Data *d)
{
	if (!d->grid || d->h <= 0 || d->w <= 0) return ;

	char **new_gen = setup_grid(d->h, d->w);

	for (int i = 0; i < d->h; i++)
	{
		for (int j = 0; j < d->w; j++)
		{
			int count = count_neighbors(d, i, j);

			if (d->grid[i][j] == 'O')
			{
				if (count == 2 || count == 3) new_gen[i][j] = 'O';
			}
			else
				if (count == 3) new_gen[i][j] = 'O';
		}
	}
	for (int i = 0; i < d->h; i++)
		for (int j = 0; j < d->w; j++) d->grid[i][j] = new_gen[i][j];
	free_grid(new_gen, d->h);
}

int	main(int ac, char **av)
{
	if (ac != 4) return 0;
	Data d = {0};

	init_data(&d, av);
	populate_grid(&d);
	for (int i = 0; i < d.iter; i++) evolve_grid(&d);
	put_grid(d.grid, d.h, d.w);
	free_grid(d.grid, d.h);
}
