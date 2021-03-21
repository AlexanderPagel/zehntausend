#include <ctime>
#include <cstdlib>


//#include "tenK.h"
#include "sarsa.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "ui.h"




#include "ref_game.h"





#ifdef TEST_RUN
    #error TEST_RUN is already defined
#else
//    #define TEST_RUN
#endif // TEST_RUN

    // game
#ifdef TEST_RUN
    int constexpr playerNo = 2;
#else
    int constexpr playerNo = 3;
#endif

double avg[playerNo] = {0};
int tries[playerNo] = {0};


double updateAvg(int pl, double total)
{
    ++tries[pl];
    return avg[pl] = total / tries[pl];
}

std::string dToS(double d)
{
    std::ostringstream oss;
    oss << d;
    return oss.str();
}

/// \tbc make an unbiased randomness generation for the dice rolls


/// \tbc define #TEST and #NOTEST to execute depending on global constexpr?

template<unsigned int n>
static void reprint(Tenthousand<n> const& game, std::string msg = "")
{
    std::cout << std::endl << std::endl;
    std::cout << std::setprecision(5) << std::fixed << msg << std::endl;
    std::cout << std::string(80,'-') << std::endl;
    game.print();
}

int main()
{
  // That worked very well last time so will indeed just smack all my new
  // testing in the beginning of main again.

  auto newG = refac::Game{};





































  // For now I will just stash all testing in fron of main and return yolo

  srand(time(nullptr));

  // Let UI factory create a new ui object
  ui::Ui* ui = ui::UiFactory{};

  ui->startGame();

















  // The simulation refactoring made pretty much all of this break
  /*
    /// \tbc test / read copy constructor calls
    /// \tbc implement random state learning

    /// \tbc make strengths < 100 TRAIN with 350 rule but play without? :D they don't know better ^^
    /// play 1 human vs. 2 bots

#ifndef TEST_RUN
    std::cout << "In-Game controls:\n"
         << std::string(72, '-') << std::endl;
    std::cout << std::setw(14) << std::left << "(1) to (6)" << "select die (deactivates it)\n"
         << std::setw(14) << std::left << "(a)" << "select All possible dice\n"
         << std::setw(14) << std::left << "(r)" << "Roll active dice\n"
         << std::setw(14) << std::left << "(f)" << "Finish your turn\n"
         << std::setw(14) << std::left << "(l)" << "get Lost (quit)\n" << std::endl;
    std::cout << "\tNote: (a) Can find new triples, but is currently unable\n\tto complete triples\n" << std::endl;
    std::cout << std::setw(14) << std::left << "\nPress any key to start." << std::endl;
//    getch(); /// \tbc quick = use setting from last timeS
    std::cin.ignore();

    // computer interaction (input parameters by player)
//    system("cls");
    std::cout << std::endl << std::endl;
    std::cout << "Choose your AI level.\n"
         << std::string(72, '-') << std::endl
         << "Note:\n\tCurrently, AI skill is limited by computing power.\n"
            "\tYou might still win against your local desktop machine.\n\n"
            "Typical levels might be:\n\n\n" << std::right
         << "\t" << std::setw( 7 ) << 10     << "\t| newborn  | hopefully doesn't shit its pants"     << std::endl
         << "\t" << std::setw( 7 ) << " "    << "\t|          |" << std::endl
         << "\t" << std::setw( 7 ) << 100    << "\t| child    | might understand the rules next year" << std::endl
         << "\t" << std::setw( 7 ) << " "    << "\t|          |" << std::endl
         << "\t" << std::setw( 7 ) << 1000   << "\t| teenager | mostly distracted but sometimes lucky af" << std::endl
         << "\t" << std::setw( 7 ) << " "    << "\t|          |" << std::endl         << "\t" << std::setw( 7 ) << 10000  << "\t| adult    | an actual opponent"<< std::endl
         << "\t" << std::setw( 7 ) << " "    << "\t|          |" << std::endl
         << "\t" << std::setw( 7 ) << 100000 << "\t| R2FU     | that fkn bot has hax" << std::endl
         << "\n\n\tAny number from 0 to 4 billion will work.\n\n\n"
            "Level: ";
#else
    std::cout << "Starting test" << std::endl;
#endif
    int steps;
#ifndef TEST_RUN
    std::cin >> steps; std::cin.sync(); std::cin.clear();
    if( steps < 0 )
        steps = 0;
#else
    steps = 0;
#endif

    std::cout << "\n\n\tStarting. Please wait..." << std::endl;
    Sarsa* bot;
#ifndef TEST_RUN
    if( steps < 1000 )
        bot = new Sarsa(0.25, .2);
    else if( steps < 10000 )
        bot = new Sarsa(0.18, .15);
    else if( steps < 100000 )
        bot = new Sarsa(0.14, .12);
    else
        bot = new Sarsa(0.12, .1);
#else
    bot = new Sarsa(0.09, 0.09);
#endif

#ifndef TEST_RUN
//    system("cls");
    std::cout << std::endl << std::endl;
    std::cout << "For level " << steps << ", the AI chose the following parameters for 1-step TD(0) control (Sarsa):\n"
            << "\n\talpha      = " << bot->alpha
            << "\n\tepsilon    = " << bot->epsilon
            << "\n\tgamma      = " << bot->gamma << std::endl;
    std::cout << "\n\n" << std::endl;
    // randomness
    std::srand(time(nullptr));
#else
    std::srand(time( nullptr ));
#endif

    // game
    Tenthousand<playerNo> game;

    // human interaction
    unsigned char in;

    // errors and info
    std::string s;

    // time management: thinking and waiting
    enum Activity { LEARN, PROCRAST };
    auto wait = [&bot, &steps](bool activity = PROCRAST, double factor = 1.0)->void
    {
        size_t start = time(nullptr);
        if( activity == LEARN )
        {
            int l = int(steps*factor),
                i = l;
            if( i < 0 )
            {
                i = 0; l= 0;
            }

            std::cout << "[Training]\t:\t" << l << std::endl;
            bot->performLearningEpisodes(l, 80);
            std::cout << std::endl;
        }

        int left = 4 - (time(nullptr) - start);
        left = left > 0 ? left : 0;
        #ifndef TEST_RUN
//        Sleep(left * int(factor * 1000)); /// \tbc lol does not work as intended

        #endif // TEST_RUN
    }; (void)wait;

#ifndef TEST_RUN
std::cout << "[Preparation]" << std::endl;
    bot->performLearningEpisodes(steps * 10, 80);
#else
//    bot->performLearningEpisodes(500000000, 80);
      bot->performLearningEpisodes(50000, 80);
    // 500 000 000 (.05/.05) -> avg 430 (200 000 total)
    // 500000000 (.06/.06) -> avg 454 (300000 total)
    // 500000000 (.07/.07) -> avg 446 (300000 total)
    // 500000000 (.09/.07) -> avg 360 (100000 total)
    // 50000000  (.05/.1)  -> avg 452 (200000 total) | SARSAMAX

#endif

    // game loop
    std::cout << "\nStarting now" << std::endl;
    while(true)
    {

        try
        {
            reprint(game,s);

            if( game.getPlayer() != 0 )
            {
                // - humans turn
                if( game.getPlayer() == 1 )
                    if( game.getCurrent() == 0 )
                        updateAvg(0, game.getPoints(0));    // player 1 updates avg of bot on first turn
                s = std::string("(bot) ") + dToS(avg[0]) + " -- " + dToS(avg[1]) + " (hum) ";

                bool in_again = true;
                do
                {
#ifdef TEST_RUN
                    in = 'f';
#else
//                    in = getch();
                    std::cin >> in; std::cin.sync(); std::cin.clear();
#endif // TEST_RUN

                    switch( in )
                    {
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                        game.putAside(in - '1');
                        in_again = false;
                        break;
                    case 'a':
                        game.putAside();
                        in_again = false;
                        break;
                    case 'f':
                        game.putAside();
                        game.finishTurn();
                        in_again = false;
                        break;
                    case 'r':
                        if( game.getPutAny() )
                        {
                            game.roll();
                            in_again = false;
                        }
                        break;
                    case 'l':
                        goto cleanup;
                    default:
                        std::cout << "[inv]" << std::endl;
                        continue;
                    }
                } while( in_again == true );
            }
            else
            {
                // - AI turn

                if( game.getCurrent() == 0 )
                    for(int i = 1; i < playerNo; ++i)
                        updateAvg(i, game.getPoints(i));    // update avg of other player

                s = std::string("(bot) ") + dToS(avg[0]) + " -- " + dToS(avg[1]) + " (hum) ";

                std::string m;

                Sarsa::State_t state( game.state() );

                #ifdef TEST_RUN
                Sarsa::Action_t a = bot->greedy( std::move(state), true );
                #else
                Sarsa::Action_t a = bot->greedy( std::move(state), false );
                #endif // TEST_RUN

                m += "\n\tThinking...\n";
                std::cout << m << std::endl;

                #ifndef TEST_RUN
                wait(LEARN);
                #else
//                bot->performLearningEpisodes(steps, 0);
                #endif // TEST_RUN

                if( Sarsa::Game_t::legal(state, a) != 0 )
                {
                    game.putAside( a );

                    m += "\n\tI do this.\n";
                    reprint(game);
                    std::cout << m << std::endl;
                    #ifndef TEST_RUN
                    wait(PROCRAST, .25);
                    #endif // TEST_RUN

                    if( a.finishes() )
                    {
                        m += "\n\t <-- And finish.\n";
                        reprint(game);
                        std::cout << m << std::endl;
                        #ifndef TEST_RUN

                        wait(PROCRAST, .25);
                        #endif // TEST_RUN
                        game.finishTurn();
                    }
                    else
                    {
                        m += "\n\t => And go all in.\n";
                        reprint(game);
                        std::cout << m << std::endl;
                        game.roll();
                        #ifndef TEST_RUN
                        wait(PROCRAST, .5);

                        #endif // TEST_RUN
                    }
                }
                else
                {
                    m += "\n\t - welp -";
                    reprint(game);
                    std::cout << m << std::endl;
                    game.finishTurn();
                }

            }   // bot moves
        }
        catch(illegal_move_error& err)
        {
            if( game.getPlayer() == 1 )
            {
                std::cout << "Invalid." << std::endl;

                #ifndef TEST_RUN
                wait(PROCRAST, .5);
                #endif // TEST_RUN
            }

            s += std::string(" | ") + err.what();

        }
        catch ( ... )
        {
            std::cout << "what happended???" << std::endl;
            exit(43);
        }
    }

    cleanup:
    delete bot;
    */

    return 0;
}

