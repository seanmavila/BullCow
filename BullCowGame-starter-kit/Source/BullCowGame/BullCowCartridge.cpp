// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
// #include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word)
        {
            return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word);
        });

    InitGame();       
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
     // if game is over then ClearScreen() and InitGame() the game
    

    if (bGameOver)
    {
        ClearScreen();
        InitGame();
    }
    else // Check player guess
    {
        ProcessGuess(PlayerInput);        
    }   
}

void UBullCowCartridge::InitGame()
{
    bGameOver = false;

    // Welcome players
    PrintLine(TEXT("Welcome to Bull Cow!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; // Set the hidden word to be guessed
    PlayerLives = HiddenWord.Len();

    PrintLine(TEXT("The hidden word is: %s"), *HiddenWord);
    PrintLine(TEXT("Press tab and guess an %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives! \nPress enter to continue..."), HiddenWord.Len());
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again!"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Correct! You win!"));
        EndGame();
        return;
    }

    // Check if word is right number of characters
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long..."), PlayerLives);
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining..."), PlayerLives);
        return;
    }

    
    // Check if guess is an isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters allowed, guess again"));
    }

    // Decrement life
    --PlayerLives;
    PrintLine(TEXT("You have lost a life!"));

    if (PlayerLives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s\n"), *HiddenWord);
        EndGame();
        return;
    }

    
    // Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows..."), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again, you have %i lives left..."), PlayerLives);
    
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString CurWord : WordList)
    {
        if (CurWord.Len() >= 4 && CurWord.Len() <= 8 && IsIsogram(CurWord))
        {
            ValidWords.Emplace(CurWord);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}

