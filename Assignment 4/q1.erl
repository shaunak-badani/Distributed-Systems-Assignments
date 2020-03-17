-module(q1).
-export([main/0, spawnProcesses/3, send_and_receive_token/2, send_and_receive_token/1, spawnRemainingProcesses / 4]).


send_and_receive_token(0, Token) ->
    receive
        {sender_id, PID} ->
            PID ! {token, Token, 0}
    end,
    receive
        {token, Token, Process_ID} ->
            io:format("Process 0 received token ~w from process ~w. ~n", [Token, Process_ID])
    end.
send_and_receive_token(Rank) ->
    receive
        {sender_id, PID} ->
            receive
                {token, Token, Process_ID} ->
                    io:format("Process ~w received token ~w from process ~w. ~n", [Rank, Token, Process_ID]),
                    PID ! {token, Token, Rank}
            end
    end.

spawnRemainingProcesses(Rank, N, Prev_ID, Root) when Rank < N ->
    New_ID = spawn(q1, send_and_receive_token, [Rank]),
    Prev_ID ! {sender_id, New_ID},
    spawnRemainingProcesses(Rank + 1, N, New_ID, Root);
spawnRemainingProcesses(_, _, Prev_ID, Root) ->
    Prev_ID ! {sender_id, Root}.


spawnProcesses(0, N, Token) ->
    Root = spawn(q1, send_and_receive_token, [0, Token]),
    spawnRemainingProcesses(1, N, Root, Root).


main() ->
    {ok, [NoOfProcesses, Token]} = io:fread("", "~d ~d"),
    _ = spawnProcesses(0, NoOfProcesses, Token).