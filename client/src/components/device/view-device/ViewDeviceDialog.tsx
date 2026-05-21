"use client";
import React, { useState } from "react";
import { Button } from "../../ui/button";
import { DeviceDialog } from "../shared/DeviceDialog";
import { FormProvider } from "react-hook-form";
import { Device } from "@/types";
import { useEditDeviceForm } from "../edit-device/useEditDeviceForm";
import { BaseDeviceForm } from "../shared/BaseDeviceForm";
import { IsOnSwitch } from "../shared/IsOnSwitch";

interface ViewDeviceDialogProps {
  device?: Device;
}

export default function ViewDeviceDialog({ device }: ViewDeviceDialogProps) {
  const [open, setOpen] = useState(false);
  const form = useEditDeviceForm(device);

  return (
    <FormProvider {...form}>
      <DeviceDialog
        trigger={<Button size="sm" variant="outline">Ver Detalhes</Button>}
        title="Detalhes do dispositivo"
        description="Informações do dispositivo selecionado."
        open={open}
        onOpenChange={() => setOpen(!open)}
      >
        <BaseDeviceForm disabled={true}/>
        <div className="flex flex-row justify-between items-center">
        <IsOnSwitch disabled={true}/>
        <p> Criado em {new Date().toLocaleDateString()}</p>
        </div>
        <div className="flex gap-3 pt-10 justify-end">
          <Button variant="outline" onClick={() => setOpen(false)} className="text-foreground">
            Fechar
          </Button>
        </div>
      </DeviceDialog>
    </FormProvider>
  );
}