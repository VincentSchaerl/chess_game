#include <cstdlib>
#include <ctime>
#include <iostream>
#include "input_output.h"
#include "piece.h"
#include "play.h"

int main()
{
	std::srand(std::time(0));
	bool playAgain = true;
	while (playAgain)
	{
		try
		{
			Color playerColor = inputColor();
			play(playerColor);
			playAgain = inputPlayAgain();
		}
		catch (const NewGameException&)
		{
			std::cout << "\n\nYou have started a new game.\n\n";
			continue;
		}
		catch (const ExitException&)
		{
			break;
		}
	}
	return 0;
}