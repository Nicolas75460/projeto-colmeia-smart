import { zodResolver } from "@hookform/resolvers/zod";
import { useForm } from "react-hook-form";
import { newDeviceSchema, newDeviceType } from "./validator";

export const useNewDeviceForm = () => {
  const form = useForm<newDeviceType>({
    resolver: zodResolver(newDeviceSchema),
    mode: "onChange",
    reValidateMode: "onChange",
    defaultValues: {
      name: "",
      preset: "",
    },
  });

  return form;
};
