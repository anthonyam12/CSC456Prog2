class Process
{
    public:
        int arrival_time;
        int burst_time;
        int process_id;
        int priority;
        int finish_time;
        int response_time;
        bool first_service = true;
};
