-module(q2).
-export([start/0, print_list/1, merge / 2, mergeSort / 3, receive_all/1, receive_all/3, spawnProcesses/6, sort_and_send/1, receive_sublist/1, spawn_and_send/3]).

print_list([ ]) ->
    io:format("~n");
print_list([F | Rest]) ->
    io:format("~w ", [F]),
    print_list(Rest).

% Start mergesort code

merge([], Back) ->
    Back;
merge(Front, []) ->
    Front;
merge([L | Front], [R | Back]) when L < R ->
    [L | merge(Front, [R | Back])];
merge([L | Front], [R | Back]) ->
    [R | merge([L | Front], Back)].

mergeSort(X, L, R) when L < R ->
    Mid = floor((L + R) / 2),
    Left = mergeSort(X, L, Mid),
    Right = mergeSort(X, Mid + 1, R),
    merge(Left, Right);
mergeSort(X, L, _) ->
    P = lists:nth(L, X),
    [P].

% End mergesort code

% Process wise sublist sorting

sort_and_send(SubList) ->
    L = length(SubList),
    Sorted = mergeSort(SubList, 1, L),
    Sorted.

% End Process wise sublist sorting

receive_sublist(1) ->
    receive
        {sublist, SubList} ->
            List_to_send = sort_and_send(SubList),
            root_process ! {sorted_list, List_to_send}
    end;
receive_sublist(_) ->
    receive
        {sublist, SubList} ->
            List_to_send = sort_and_send(SubList),
            root_process ! {sorted_list, List_to_send}
    end.

% The root process collects all merged sublists

receive_all(Acc_Proc, Array_Acc, No_of_processes) when Acc_Proc == No_of_processes ->
    io:format("Collected array : ~w ~n", [Array_Acc]);
receive_all(Acc_Proc, Array_Acc, No_of_processes) ->
    receive
        {sorted_list, SubList} ->
            Merged = merge(Array_Acc, SubList),
            receive_all(Acc_Proc + 1, Merged, No_of_processes)
    end.

receive_all(No_of_processes) ->
    receive_all(0, [], No_of_processes).

% End collection

spawn_and_send(SubList, 1, No_of_processes) ->
    register(root_process, spawn(q2, receive_all, [No_of_processes])),
    Root_Pid = spawn(q2, receive_sublist, [1]),
    Root_Pid ! {sublist, SubList};
spawn_and_send(SubList, Rank, _) ->
    PID = spawn(q2, receive_sublist, [Rank]),
    PID ! {sublist, SubList}.

% Spawn processes 

spawnProcesses(Current_Rank, Array, [], Numbers_Per_Proc, Len, No_of_processes) ->
    [Current | Rest ] = Array,
    Arr_Acc = [Current],
    Len_dup = Len + 1,
    spawnProcesses(Current_Rank, Rest, Arr_Acc, Numbers_Per_Proc, Len_dup, No_of_processes);
spawnProcesses(Current_Rank, [], Arr_Acc, __, _, No_of_processes) ->
    % io:format("~w ~w ~n", [Current_Rank, Arr_Acc]),
    spawn_and_send(Arr_Acc, Current_Rank, No_of_processes);
spawnProcesses(Current_Rank, Array, Arr_Acc, Numbers_Per_Proc, Len, No_of_processes) ->
    if 
        Current_Rank == No_of_processes ->
            [Current | Rest] = Array,
            Arr_A = lists:append(Arr_Acc, [Current]),
            Len_dup = Len + 1,
            spawnProcesses(Current_Rank, Rest, Arr_A, Numbers_Per_Proc, Len_dup, No_of_processes);
        Len == Numbers_Per_Proc ->
            spawn_and_send(Arr_Acc, Current_Rank, No_of_processes),
            % io:format("~w ~w ~n", [Current_Rank, Arr_Acc]),
            Arr_B = [],
            Current_Rank_dup = Current_Rank + 1,
            Len_dup_1 = 0,  
            spawnProcesses(Current_Rank_dup, Array, Arr_B, Numbers_Per_Proc, Len_dup_1, No_of_processes);
        true ->
            [Current | Rest] = Array,
            Arr_A = lists:append(Arr_Acc, [Current]),
            Len_dup = Len + 1,
            spawnProcesses(Current_Rank, Rest, Arr_A, Numbers_Per_Proc, Len_dup, No_of_processes)
    end.

% End spawning of processes

start() ->
    X = [10, 12, 69, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 20, 88, 87, 100, 102, 103],
    L = length(X),
    No_of_processes = 8,
    % print_list(X),
    Numbers_Per_Proc = erlang:list_to_integer(erlang:float_to_list(L / No_of_processes ,[{decimals,0}])),
    % io:format("~w ~n", [Numbers_Per_Proc]),
    spawnProcesses(1, X, [], Numbers_Per_Proc, 0, No_of_processes).