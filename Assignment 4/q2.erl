-module(q2).
-export([start/0, print_list/1, merge/3, merge / 2, mergeSort / 3]).

print_list([ ]) ->
    io:format("~n");
print_list([F | Rest]) ->
    io:format("~w ", [F]),
    print_list(Rest).

% Start mergesort code

merge(A, [], R) ->
    lists:append(A, R);
merge(A, L, []) -> 
    lists:append(A, L);
merge(A, [L1 | R1], [L2 | R2]) when L1 =< L2 ->
    merge(lists:append(A, [L1]), R1, [L2, R2]);
merge(A, [L1 | R1], [L2 | R2]) ->
    merge(lists:append(A, [L2]), [L1 | R1], R2).
merge(L, R) ->
    merge([], L, R).

mergeSort(X, L, R) when L < R ->
    Mid = floor((L + R) / 2),
    Left = mergeSort(X, L, Mid),
    Right = mergeSort(X, Mid + 1, R),
    merge(Left, Right);
mergeSort(X, L, _) ->
    P = lists:nth(L, X),
    [P].

% End mergesort code

start() ->
    X = [69, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1],
    print_list(X),
    L = length(X),
    Sorted = mergeSort(X, 1, L),
    L_8 = lists:nth(11, X),

    io:format("Print X ~w ~w ~w ~n", [Sorted, L, L_8]).