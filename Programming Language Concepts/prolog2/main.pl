:- module(main, [salad_type/2, person_can_eat/3, possible_order_options/3, count_per_type/2]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE


salad_type_helper([], vegan, 0).
salad_type_helper([], vegetarian, 0).
salad_type_helper([], omnivorous, 0).
salad_type_helper([], vegetarian, 1).
salad_type_helper([], omnivorous, 1).
salad_type_helper([], omnivorous, 2).

salad_type_helper([Item|ItemList], Type, 0):-  %VEGAN
    not(not_vegan(Item)),
    not(not_vegetarian(Item)),
    salad_type_helper(ItemList, Type, 0).

salad_type_helper([Item|ItemList], Type, 0):-  %VEGAN
    not_vegan(Item),
    not(not_vegetarian(Item)),
    salad_type_helper(ItemList, Type, 1).

salad_type_helper([Item|ItemList], Type, 0):-  %VEGAN
    not_vegan(Item),
    not_vegetarian(Item),
    salad_type_helper(ItemList, Type, 2).


salad_type_helper([Item|ItemList], Type, 1):- %VEGETARIAN
    not(not_vegetarian(Item)),
    salad_type_helper(ItemList, Type, 1).
    

salad_type_helper([Item|ItemList], Type, 1):- %VEGETARIAN
    not_vegetarian(Item),
    salad_type_helper(ItemList, Type, 2).


salad_type_helper([_|ItemList], Type, 2):- %OMNIVORIOUS
    salad_type_helper(ItemList, Type, 2).


salad_type([Item|ItemList], Type) :-
    base(Item),
    salad_type_helper(ItemList, Type, 0).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

person_can_eat_helper(_,[], Liste, Liste).

person_can_eat_helper(Person, [H|SaladList], Liste, Listo) :-
    vegan(Person),
    salad_type(H, vegan),
    append(Listo, [H], YeniListe),
    !,
    person_can_eat_helper(Person, SaladList, Liste, YeniListe).
    

person_can_eat_helper(Person, [H|SaladList], Liste, Listo) :-
    vegetarian(Person),
    salad_type(H, vegetarian),
    append(Listo, [H], YeniListe),
    !,
    person_can_eat_helper(Person, SaladList, Liste, YeniListe).

person_can_eat_helper(Person, [H|SaladList], Liste, Listo) :-
    vegetarian(Person),
    salad_type(H, vegan),
    append(Listo, [H], YeniListe),
    !,
    person_can_eat_helper(Person, SaladList, Liste, YeniListe).


person_can_eat_helper(Person, [H|SaladList], Liste, Listo) :-
    omnivorous(Person),
    salad_type(H, vegan),
    append(Listo, [H], YeniListe),
    !,
    person_can_eat_helper(Person, SaladList, Liste, YeniListe).

person_can_eat_helper(Person, [H|SaladList], Liste, Listo) :-
    omnivorous(Person),
    salad_type(H, vegetarian),
    append(Listo, [H], YeniListe),
    !,
    person_can_eat_helper(Person, SaladList, Liste, YeniListe).

person_can_eat_helper(Person, [H|SaladList], Liste, Listo) :-
    omnivorous(Person),
    salad_type(H, omnivorous),
    append(Listo, [H], YeniListe),
    !,
    person_can_eat_helper(Person, SaladList, Liste, YeniListe).
    
    


person_can_eat(Person, SaladList, Salad) :-
    person_can_eat_helper(Person,SaladList, Liste, []), member(Salad, Liste).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

possible_order_options_helper(_, _, _, []).

possible_order_options_helper(Person, b, ItemList, [Order|OrderChars]):-
    base(X),
    append(ItemList, [X], NewItemList),
    possible_order_options_helper(Person, Order, NewItemList, OrderChars).


possible_order_options_helper(Person, p, ItemList, [Order|OrderChars]):-
    protein(X),
    append(ItemList, [X], NewItemList),
    possible_order_options_helper(Person, Order, NewItemList, OrderChars).


possible_order_options_helper(Person, t, ItemList, [Order|OrderChars]):-
    topping(X),
    append(ItemList, [X], NewItemList),
    possible_order_options_helper(Person, Order, NewItemList, OrderChars).


possible_order_options_helper(Person, c, ItemList, [Order|OrderChars]):-
    condiment(X),
    append(ItemList, [X], NewItemList),
    possible_order_options_helper(Person, Order, NewItemList, OrderChars).


possible_order_options_helper(Person, s, ItemList, [Order|OrderChars]):-
    sauce(X),
    append(ItemList, [X], NewItemList),
    possible_order_options_helper(Person, Order, NewItemList, OrderChars).


possible_order_options(Person, OrderString, ItemList) :-
    atom_chars(OrderString, [Char|Chars]),
    possible_order_options_helper(Person, Char, ItemList, Chars).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

count_per_type([], []).
count_per_type(SaladList, TypeList) :-
    true.

