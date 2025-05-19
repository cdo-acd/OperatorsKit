from outflank_stage1.task.base_bof_task import BaseBOFTask


class CaptureNetNTLMBof(BaseBOFTask):
    def __init__(self):
        super().__init__("capturenetntlm")

        self.parser.description = (
            "Capture the NetNTLMv2 hash of the current user."
        )
