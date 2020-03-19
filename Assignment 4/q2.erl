-module(q2).
-export([main/1, print_to_file/3, print_list/2, merge / 2, mergeSort / 3, receive_all/2, receive_all/4, spawnProcesses/7, sort_and_send/1, receive_sublist/1, spawn_and_send/4, collect_array_from_file/2]).

print_to_file(Outp_filename, StrVal,  Arr) ->
    file:write_file(Outp_filename, io_lib:fwrite(StrVal, Arr), [append]).

print_list([ ], Outp_filename) ->
    print_to_file(Outp_filename, "~n", []);
print_list([F | Rest], Outp_filename) ->
    print_to_file(Outp_filename, "~w ", [F]),
    print_list(Rest, Outp_filename).

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

receive_all(Acc_Proc, Array_Acc, No_of_processes, Outp_filename) when Acc_Proc == No_of_processes ->
    print_list(Array_Acc, Outp_filename);
receive_all(Acc_Proc, Array_Acc, No_of_processes, Outp_filename) ->
    receive
        {sorted_list, SubList} ->
            Merged = merge(Array_Acc, SubList),
            receive_all(Acc_Proc + 1, Merged, No_of_processes, Outp_filename)
    end.

receive_all(No_of_processes, Outp_filename) ->
    receive_all(0, [], No_of_processes, Outp_filename).

% End collection

spawn_and_send(SubList, 1, No_of_processes, Outp_filename) ->
    register(root_process, spawn(q2, receive_all, [No_of_processes, Outp_filename])),
    Root_Pid = spawn(q2, receive_sublist, [1]),
    Root_Pid ! {sublist, SubList};
spawn_and_send(SubList, Rank, _, _) ->
    PID = spawn(q2, receive_sublist, [Rank]),
    PID ! {sublist, SubList}.

% Spawn processes 

spawnProcesses(Current_Rank, Array, [], Numbers_Per_Proc, Len, No_of_processes, Outp_filename) ->
    [Current | Rest ] = Array,
    Arr_Acc = [Current],
    Len_dup = Len + 1,
    spawnProcesses(Current_Rank, Rest, Arr_Acc, Numbers_Per_Proc, Len_dup, No_of_processes, Outp_filename);
spawnProcesses(Current_Rank, [], Arr_Acc, __, _, No_of_processes, Outp_filename) ->
    spawn_and_send(Arr_Acc, Current_Rank, No_of_processes, Outp_filename);
spawnProcesses(Current_Rank, Array, Arr_Acc, Numbers_Per_Proc, Len, No_of_processes, Outp_filename) ->
    if 
        Current_Rank == No_of_processes ->
            [Current | Rest] = Array,
            Arr_A = lists:append(Arr_Acc, [Current]),
            Len_dup = Len + 1,
            spawnProcesses(Current_Rank, Rest, Arr_A, Numbers_Per_Proc, Len_dup, No_of_processes, Outp_filename);
        Len == Numbers_Per_Proc ->
            spawn_and_send(Arr_Acc, Current_Rank, No_of_processes, Outp_filename),
            Arr_B = [],
            Current_Rank_dup = Current_Rank + 1,
            Len_dup_1 = 0,  
            spawnProcesses(Current_Rank_dup, Array, Arr_B, Numbers_Per_Proc, Len_dup_1, No_of_processes, Outp_filename);
        true ->
            [Current | Rest] = Array,
            Arr_A = lists:append(Arr_Acc, [Current]),
            Len_dup = Len + 1,
            spawnProcesses(Current_Rank, Rest, Arr_A, Numbers_Per_Proc, Len_dup, No_of_processes, Outp_filename)
    end.

% End spawning of processes

collect_array_from_file(Filename, _) ->
    {ok, FileBuffer} = file:open(Filename, [read]),
    {ok, Line} = file:read_line(FileBuffer),
    StrNos = string:tokens(string:strip(Line, right, 10), [$\s]),
    X = lists:map(fun erlang:list_to_integer/1, StrNos),
    X.

main([Inp_filename | Outp_filename]) ->
    file:delete(Outp_filename),
    io:format("~w ~n", [Outp_filename]),
    X = collect_array_from_file(Inp_filename, Outp_filename),
    L = length(X),
    No_of_processes = 8,
    Numbers_Per_Proc = erlang:list_to_integer(erlang:float_to_list(L / No_of_processes ,[{decimals,0}])),
    spawnProcesses(1, X, [], Numbers_Per_Proc, 0, No_of_processes, Outp_filename).