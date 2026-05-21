import { useForm } from "react-hook-form";
import { editDeviceSchema, EditDeviceType } from "./validator";
import { zodResolver } from "@hookform/resolvers/zod";
import { Device } from "@/types";

export const useEditDeviceForm = (device?: Device) => {
  const form = useForm<EditDeviceType>({
    mode: "onChange",
    reValidateMode: "onChange",
    resolver: zodResolver(editDeviceSchema),

    // TODO: adicionar preset e is_active na interface e retorno da API
    defaultValues: {
      id: device?.id || "",
      name: device?.name || "",
      preset: "",
      is_active: true,
    },
  });

  return form;
};
