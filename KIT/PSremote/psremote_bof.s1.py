from typing import List, Tuple

from outflank_stage1.task.base_bof_task import BaseBOFTask
from outflank_stage1.task.enums import BOFArgumentEncoding
from outflank_stage1.task.tasks import DownloadTask


class PSRemoteBOF(BaseBOFTask):
    def __init__(self):
        super().__init__("psremote")

        self.parser.description = (
            "List all running processes on a remote host."
        )
        self.parser.epilog = "Usage: psremote <remotehost>"

        self.parser.add_argument(
            "remotehost",
            help=f"FQDN or IP Address of remote host"
        )

    def _encode_arguments_bof(
        self, arguments: List[str]
    ) -> List[Tuple[BOFArgumentEncoding, str]]:
        parser_arguments = self.parser.parse_args(arguments)

        return [(BOFArgumentEncoding.STR, parser_arguments.remotehost)]
