from outflank_stage1.task.base_bof_task import BaseBOFTask


class PSRemoteBOF(BaseBOFTask):
    def __init__(self):
        super().__init__("psremote")

        self.parser.add_argument("remotehost", help="FQDN or IP remote host.")

        self.parser.description = "List all running processes on a remote host."

        self.parser.epilog = "Example usage:\n  - psremote dc-2.worldcom.lab\n  - psremote 10.0.10.10\n"

    def _encode_arguments_bof(self, arguments: List[str]) -> List[Tuple[BOFArgumentEncoding, str]]:
        parser_arguments = self.parser.parse_args(arguments)
        return [(BOFArgumentEncoding.STR, parser_arguments.remotehost)]
