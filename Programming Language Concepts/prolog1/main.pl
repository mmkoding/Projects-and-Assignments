:- module(main, [is_movie_directed_by/2, total_awards_nominated/2, all_movies_directed_by/2, total_movies_released_in/3, all_movies_released_between/4]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

is_movie_directed_by(Title, Director) :-
    movie(Title, Director, _, _, _, _).
     
total_awards_nominated(Title, Nominations):-
    movie(Title, _, _, A, B, C), 
    Nominations is (A + B + C).
    

all_movies_directed_by(Director, Movies):-
    findall(Name, movie(Name, Director, _, _, _, _), Movies).
    

total_movies_released_in([],_,0).
total_movies_released_in([Movie|Movies],Year,Count):-
    movie(Movie, _, Year, _, _, _),
    !,
    total_movies_released_in(Movies, Year, Countpp),
    Count is Countpp -(-1).
total_movies_released_in([_|Movies], Year, Count) :-
    total_movies_released_in(Movies, Year, Count).
    
    
all_movies_released_between_helper([],_,_,MoviesBetweenGivenYears, MoviesBetweenGivenYears).
all_movies_released_between_helper([Movie|Movies],MinYear,MaxYear,MoviesBetweenGivenYears, Container):-
    movie(Movie,_,Year,_,_,_),
    MinYear > Year,
    all_movies_released_between_helper(Movies,MinYear,MaxYear,MoviesBetweenGivenYears, Container).
    
all_movies_released_between_helper([Movie|Movies],MinYear,MaxYear,MoviesBetweenGivenYears, Container):-
    movie(Movie,_,Year,_,_,_),
    MaxYear < Year,
    all_movies_released_between_helper(Movies,MinYear,MaxYear,MoviesBetweenGivenYears, Container).
    
all_movies_released_between_helper([Movie|Movies],MinYear,MaxYear,MoviesBetweenGivenYears, Container):-
    movie(Movie,_,Year,_,_,_),
    Year >= MinYear,
    Year =< MaxYear,
    append(Container, [Movie], NewContainer),
    all_movies_released_between_helper(Movies,MinYear,MaxYear,MoviesBetweenGivenYears, NewContainer).


all_movies_released_between(Movies,MinYear,MaxYear,MoviesBetweenGivenYears):-
    all_movies_released_between_helper(Movies,MinYear,MaxYear,MoviesBetweenGivenYears, []).
    
