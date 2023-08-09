from outflank_stage1.task.base_bof_task import BaseBOFTask


class EnumSecProductsBOF(BaseBOFTask):
    def __init__(self):
        super().__init__("enumsecproducts")

        self.parser.description = "List security products (like AV/EDR) that are running on the system."
