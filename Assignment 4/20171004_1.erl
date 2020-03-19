-module('20171004_1').
-compile(export_all).

print_to_file(Outp_filename, StrVal,  Arr) ->
    file:write_file(Outp_filename, io_lib:fwrite(StrVal, Arr), [append]).

% The root process first accepts the PID of the next process.
% It then sends out the token to the next process, and 
% Waits to receive the token from the N-1th process.

send_and_receive_token(0, Token, Outp_filename) ->
    receive
        {sender_id, PID} ->
            PID ! {token, Token, 0}
    end,
    receive
        {token, Token, Process_ID} ->
            print_to_file(Outp_filename, "Process 0 received token ~w from process ~w. ~n", [Token, Process_ID])
    end.
send_and_receive_token(Rank, Outp_filename) ->
    receive
        {sender_id, PID} ->
            receive
                {token, Token, Process_ID} ->
                    print_to_file(Outp_filename, "Process ~w received token ~w from process ~w. ~n", [Rank, Token, Process_ID]),
                    PID ! {token, Token, Rank}
            end
    end.

% Params : 
% Rank : Rank of the process spawned. Gives identity to process apart from PID.
% N : No of processes to be spawned.
% Prev_ID : The Process ID which is to be sent to the process spawned in current iteration.
% Root : Process ID of the first process
% Outp_filename : Self Explanatory

spawnRemainingProcesses(Rank, N, Prev_ID, Root, Outp_filename) when Rank < N ->
    New_ID = spawn(?MODULE, send_and_receive_token, [Rank, Outp_filename]),
    Prev_ID ! {sender_id, New_ID},
    spawnRemainingProcesses(Rank + 1, N, New_ID, Root, Outp_filename);
spawnRemainingProcesses(_, _, Prev_ID, Root, _) ->
    Prev_ID ! {sender_id, Root}.

% Spawn processes has a special definition for the 0th rank as it  
% must accept the token. The token must not be passed to any other 
% process when it is spawned. It will receive the token from its
% predecessor.

spawnProcesses(0, N, Token, Outp_filename) ->
    Root = spawn(?MODULE, send_and_receive_token, [0, Token, Outp_filename]),
    spawnRemainingProcesses(1, N, Root, Root, Outp_filename).


main([ Inp_Filename | Outp_filename]) ->
    file:delete(Outp_filename),
    {ok, FileReadBuffer} = file:open(Inp_Filename, [read]),
    {ok, [NoOfProcesses, Token]} = io:fread(FileReadBuffer, "", "~d ~d"),
    _ = spawnProcesses(0, NoOfProcesses, Token, Outp_filename).